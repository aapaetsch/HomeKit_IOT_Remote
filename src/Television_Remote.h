#include <HomeSpan.h>
#include <list>
// TODO: Update the library call..
#include <IRremote.hpp>

/// @brief A Television Speaker Service
/// @details This service describes a speaker that is part of a television.
/// It is used in conjunction with the Television service to provide a
/// complete television accessory.
struct Television_Speaker : Service::TelevisionSpeaker {
    SpanCharacteristic *volume;
    SpanCharacteristic *volumeControlType;
    // TODO: Update mute... Does not work in the current implementation
    // SpanCharacteristic *mute;
    
    /// @brief Construct a new Television Speaker object
    Television_Speaker() : Service::TelevisionSpeaker() {
        try {
            volume = new Characteristic::VolumeSelector();
            // mute = new Characteristic::Mute(0);
            volumeControlType = new Characteristic::VolumeControlType(1);
            Serial.printf("Configured TV Speaker");
        } catch (const std::exception& e) {
            Serial.println(e.what());
        }
    } 

    boolean update() override {
        try {
            if (volume->updated() && volume->timeVal() > 100) {
                Serial.printf("Set TV Speaker Volume to: %d\n", volume->getNewVal());
                int newVal = volumeControlType->getNewVal();
                // Speaker is linked to a different remote than the TV.... Tv remote is currently not used for sound.
                if (newVal == 0) {
                    RemoteCommands::EdifierRemotePulse(RemoteCommands::EdifierRemoteCommand(EDIFIER_SPEAKER_COMMANDS::VOLUME_UP), 1);
                } else if (newVal == 1) {
                    RemoteCommands::EdifierRemotePulse(RemoteCommands::EdifierRemoteCommand(EDIFIER_SPEAKER_COMMANDS::VOLUME_DOWN), 1);
                }
            }
            return(true);
        }
        catch(const std::exception& e)
        {
            Serial.println(e.what());
            return false;
        }
    }
};

/// @brief A Television Remote Service
/// @details This service describes a remote control for a television.
/// It is used in conjunction with the Television service to provide a
/// complete television accessory.
struct Television_Remote : Service::Television {
    SpanCharacteristic *active; 
    SpanCharacteristic *activeID;
    SpanCharacteristic *remoteKey;
    SpanCharacteristic *settingsKey;
    // SpanCharacteristic *sleepDiscoveryMode;
    SpanCharacteristic *tvName;

    Television_Speaker *speakerService;
    // SpanCharacteristic *mute;
    // SpanCharacteristic *volume;
    // SpanCharacteristic *volumeControlType;
    unsigned long previousMillis = 0;
    // For IR
    int delay = 500;

    std::list<SpanService *> inputs;

    Television_Remote(const char *name, const std::list<std::pair<const char*, int>>& inputList) : Service::Television() {
        configureRemote(name);
        configureSpeakers();
        configureInputs(inputList);
    }

    void configureRemote(const char *name) {
        try {
            active = new Characteristic::Active(0);
            // TODO: Get Data from google potentially on the active input.
            // Sets Input 1 as selected input on startup
            activeID = new Characteristic::ActiveIdentifier(1);  
            remoteKey = new Characteristic::RemoteKey();     
            settingsKey = new Characteristic::PowerModeSelection();
            tvName = new Characteristic::ConfiguredName(name);             // Name of TV
            // sleepDiscoveryMode = new Characteristic::SleepDiscoveryMode(1); // Enable Sleep Discovery Mode
            Serial.printf("Configured TV: %s\n",name);
        } catch (const std::exception& e) {
            Serial.println(e.what());
        }    
    }

    void configureSpeakers() {
        try {
            speakerService = new Television_Speaker();
            // mute = speakerService->mute;
            // volume = speakerService->volume;
            // volumeControlType = speakerService->volumeControlType;
            addLink(speakerService);
            Serial.printf("\tConfigured TV Speaker\n");
        } catch (const std::exception& e) {
            Serial.println(e.what());
        }
    }

    SpanService* configureInput(const char *name, int id, bool includeInSettingsScreen = true, bool isVisible = true, bool visibilityIsTogglable = true) {
        SpanService *inputService = new Service::InputSource();
        SpanCharacteristic *inputName = new Characteristic::ConfiguredName(name);
        SpanCharacteristic *inputID = new Characteristic::Identifier(id);
        if (includeInSettingsScreen) {
            new Characteristic::IsConfigured(1);
        } else {
            new Characteristic::IsConfigured(0);
        }
        if (isVisible) {
            new Characteristic::CurrentVisibilityState(0);
            if (visibilityIsTogglable) {
                new Characteristic::TargetVisibilityState(0);
            } else {
                new Characteristic::TargetVisibilityState(1);
            }
        } else {
            new Characteristic::CurrentVisibilityState(1);
            new Characteristic::TargetVisibilityState(1);
        }
        return inputService;
    }

    void configureInputs(const std::list<std::pair<const char*, int>>& inputList) {
        try {
            for (auto input : inputList) {
                SpanService *inputService = configureInput(input.first, input.second);
                inputs.push_back(inputService);
                addLink(inputService);
            }
            Serial.printf("\tConfigured TV Inputs\n");
        } catch (const std::exception& e) {
            Serial.println(e.what());
        }
    }

    /// @brief Update the active input source
    /// @details This method is called when the active input source is changed.
    // void updateInputSource() {
    //     int newID = activeID->getNewVal();
    //     activeID->setVal(newID);
    // }

    /// @brief Update the state of the television
    /// @details This method is called when the state of the television is changed.
    boolean update() override {
        bool wasUpdated = false;
        if(active->updated() && active->timeVal() >= 1000 && active->getNewVal() != active->getVal()) {
            wasUpdated = true;
            if (active->getNewVal() == 1) {
                Serial.printf("Set TV Power to: ON\n");
            } else {
                Serial.printf("Set TV Power to: OFF\n");
            }
            RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::POWER));
        }

        // TODO: Define input source values as part of class so that they can be used here
        if(activeID->updated() && activeID->timeVal() >= 1000) {
            Serial.printf("Set Input Source to HDMI-%d\n",activeID->getNewVal());        
            switch(activeID->getNewVal()) {
                case 1: 
                    break;
                case 4:
                    // Amazon Prime
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::AMAZON_PRIME));
                    break;
                case 5:
                    // Youtube
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::YOUTUBE));
                    break;
                default:
                    Serial.printf("Input Source not recognized: %d\n", activeID->getNewVal());
                    break;
            }
        }

        // TODO: Either settings or info will need to be updated for home button... Otherwise an input source will need to be used for it
        if(settingsKey->updated() && settingsKey->timeVal() >= 1000){
            Serial.printf("Received request to \"View TV Settings\"\n");
            // Settings 
            RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::SETTINGS));
        }
        
        if(remoteKey->updated() && remoteKey->timeVal() >= 200){
            int newVal = remoteKey->getNewVal();
            Serial.printf("Remote Control key pressed: %d ", newVal);
            switch(newVal){
                case 4:
                    Serial.printf("UP ARROW\n");
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::ARROW_UP));
                    break;
                case 5:
                    Serial.printf("DOWN ARROW\n");
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::ARROW_DOWN));
                    break;
                case 6:
                    Serial.printf("LEFT ARROW\n");
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::ARROW_LEFT));
                    break;
                case 7:
                    Serial.printf("RIGHT ARROW\n");
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::ARROW_RIGHT));
                    break;
                
                case 9:
                    Serial.printf("BACK\n");
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::BACK));
                    break;
                
                case 8:
                    Serial.printf("SELECT\n");
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::SELECT));
                    break;
                case 11:
                    Serial.printf("PLAY/PAUSE\n");
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::PLAY_PAUSE));
                    break;
                case 15:
                    Serial.printf("INFO\n");
                    // Quick Menu, top left
                    RemoteCommands::TclTvRemotePulse(RemoteCommands::TclTvRemoteCommand(TCL_TV_COMMANDS::INFO));
                    break;
                default:
                    Serial.print("UNKNOWN KEY\n");
            }
        }

        // if (mute->updated()) {
        //     Serial.printf("Set TV Speaker Mute to: %s\n", mute->getNewVal()?"ON":"OFF");
        // }

        // if (volume->updated()) {
        //     Serial.printf("Set TV Speaker Volume to: %d\n", volume->getNewVal());
        // }
        // if (sleepDiscoveryMode->updated()) {
        //     Serial.printf("Set TV Sleep Discovery Mode to: %d\n", sleepDiscoveryMode->getNewVal());
        // }
        
        return(true);
    }
};

