#include <HomeSpan.h>
#include <IRremote.hpp>

struct Speaker_Remote : Service::Speaker {
    SpanCharacteristic *volume;
    SpanCharacteristic *mute;
    SpanCharacteristic *name;

    Speaker_Remote(const char *speakerName) : Service::Speaker() {
        try {
            name = new Characteristic::Name(speakerName);
            volume = new Characteristic::Volume(0);
            mute = new Characteristic::Mute(0);
            Serial.printf("Configured Speaker: %s\n", speakerName);
        } catch (const std::exception& e) {
            Serial.println(e.what());
        }
    }

    bool update() override {
        if (volume->updated()) {
            Serial.printf("Volume: %d\n", volume->getVal());
            if (volume->getNewVal() == 0) {
                RemoteCommands::EdifierRemotePulse(RemoteCommands::EdifierRemoteCommand(EDIFIER_SPEAKER_COMMANDS::VOLUME_UP), 1);
            }
            if (volume->getNewVal() == 1) {
                RemoteCommands::EdifierRemotePulse(RemoteCommands::EdifierRemoteCommand(EDIFIER_SPEAKER_COMMANDS::VOLUME_DOWN), 1);
            }
            
        }
        if (mute->updated()) {
            Serial.printf("Mute: %d\n", mute->getVal());
            RemoteCommands::EdifierRemotePulse(RemoteCommands::EdifierRemoteCommand(EDIFIER_SPEAKER_COMMANDS::MUTE), 1);
        }
        return(true);
    }
};