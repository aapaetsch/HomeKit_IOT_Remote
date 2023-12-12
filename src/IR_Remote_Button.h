


struct Edifier_Remote_Button : Service::Switch {
    SpanCharacteristic *name;
    SpanCharacteristic *on;
    unsigned long delay;
    EDIFIER_SPEAKER_COMMANDS command;
    uint8_t necCommand;

    Edifier_Remote_Button(const char* buttonName,  EDIFIER_SPEAKER_COMMANDS command, unsigned long debounceDelay = 50) : Service::Switch() {
        name = new Characteristic::Name(buttonName);
        on = new Characteristic::On();
        delay = debounceDelay;
        necCommand = RemoteCommands::EdifierRemoteCommand(command);
    }

    void turnOff() {
        on->setVal(false);
    }

    boolean update() override {
        try
        {
            int newVal = on->getNewVal();
            if (on->updated() && on->getVal() != newVal && newVal == 1) {
                if (on->timeVal() >= delay) {
                    Serial.printf("Button %s was pushed\n", name->getVal());
                    if (command == EDIFIER_SPEAKER_COMMANDS::PAIR_BT) {
                        RemoteCommands::EdifierRemotePulse(necCommand, RemoteCommands::pairBTRepeats);
                    } else {
                        RemoteCommands::EdifierRemotePulse(necCommand, 1);
                    }
                } else {
                    Serial.printf("Button %s was pushed too soon\n", name->getVal());
                }
            }
            return(true);
        }
        catch(const std::exception& e)
        {
            Serial.println(e.what());
        }
        return false;
    }
};