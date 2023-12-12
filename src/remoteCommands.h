#include <IRremote.hpp>

enum TCL_TV_COMMANDS {
    POWER,
    // VOLUME_UP,
    // VOLUME_DOWN,
    AMAZON_PRIME,
    YOUTUBE,
    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    SELECT,
    BACK,
    PLAY_PAUSE,
    INFO,
    SETTINGS
};

enum EDIFIER_SPEAKER_COMMANDS {
    SPEAKER_POWER,
    VOLUME_UP,
    VOLUME_UP_2,
    VOLUME_DOWN,
    VOLUME_DOWN_2,
    MUTE,
    INPUT_PC,
    INPUT_AUX,
    INPUT_OPT,
    INPUT_COX,
    INPUT_BT,
    PAIR_BT,
    PREVIOUS,
    NEXT,
    PLAY_PAUSE_SPEAKER
};

class RemoteCommands {
public:
    static const uint16_t edifierAddress = 0xE710;
    static const int_fast8_t pairBTRepeats = 20;
    static void TclTvRemotePulse(IRRawDataType data, uint16_t repeatPeriodMillis = 500, int_fast8_t repeats = 1) {
        IrSender.sendPulseDistanceWidth(38, 4100, 3950, 550, 1950, 550, 950, data, 24, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, repeats);    
    }

    static IRRawDataType TclTvRemoteCommand(TCL_TV_COMMANDS command) {
        switch(command) {
            case TCL_TV_COMMANDS::POWER:
                return 0xAB054F;
            case TCL_TV_COMMANDS::AMAZON_PRIME:
                return 0x7C083F;
            case TCL_TV_COMMANDS::YOUTUBE:
                return 0xB8047F;
            case TCL_TV_COMMANDS::ARROW_UP:
                return 0x6509AF;
            case TCL_TV_COMMANDS::ARROW_DOWN:
                return 0xE501AF;   
            case TCL_TV_COMMANDS::ARROW_LEFT:
                return 0x9506AF;
            case TCL_TV_COMMANDS::ARROW_RIGHT:
                return 0x150EAF;
            case TCL_TV_COMMANDS::BACK:
                return 0x1B0E4F;
            case TCL_TV_COMMANDS::SELECT:
            case TCL_TV_COMMANDS::PLAY_PAUSE:
                return 0xD002FF;
            case TCL_TV_COMMANDS::INFO:
                return 0xC103EF;
            case TCL_TV_COMMANDS::SETTINGS:
                return 0xC0F3F;
            default:
                Serial.println("TclTvRemoteCommand: Unknown command");
                break;
        }
        return NULL;
    }

    static void EdifierRemotePulse(uint8_t command, int_fast8_t repeats = 1) {
        IrSender.sendNEC(RemoteCommands::edifierAddress, command, repeats);
    }

    static uint8_t EdifierRemoteCommand(EDIFIER_SPEAKER_COMMANDS command) {
        switch(command) {
            case EDIFIER_SPEAKER_COMMANDS::SPEAKER_POWER:
                return 0x46;
            case EDIFIER_SPEAKER_COMMANDS::VOLUME_UP:
                return 0x5;
            case EDIFIER_SPEAKER_COMMANDS::VOLUME_UP_2:
                return 0x6;
            case EDIFIER_SPEAKER_COMMANDS::VOLUME_DOWN:
                return 0x47;
            case EDIFIER_SPEAKER_COMMANDS::VOLUME_DOWN_2:
                return 0x49;
            case EDIFIER_SPEAKER_COMMANDS::MUTE:
                return 0x41;
            case EDIFIER_SPEAKER_COMMANDS::INPUT_PC:
                return 0x7;
            case EDIFIER_SPEAKER_COMMANDS::INPUT_AUX:
                return 0x9;
            case EDIFIER_SPEAKER_COMMANDS::INPUT_OPT:
                return 0x45;
            case EDIFIER_SPEAKER_COMMANDS::INPUT_COX:
                return 0x3;
            case EDIFIER_SPEAKER_COMMANDS::PAIR_BT:
            case EDIFIER_SPEAKER_COMMANDS::INPUT_BT:
                return 0x5C;
            case EDIFIER_SPEAKER_COMMANDS::PREVIOUS:
                return 0x1E;
            case EDIFIER_SPEAKER_COMMANDS::NEXT:
                return 0x2;
            case EDIFIER_SPEAKER_COMMANDS::PLAY_PAUSE_SPEAKER:
                return 0x5E;
            default:
                Serial.println("EdifierRemoteCommand: Unknown command");
                break;
        }
        return NULL;
    }
};