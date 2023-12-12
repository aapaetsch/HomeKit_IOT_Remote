#include <HomeSpan.h>
#include <IRremote.hpp>
struct Honeywell_Fan_Remote : Service::Fan {
    
    SpanCharacteristic *active;
    SpanCharacteristic *name;
    SpanCharacteristic *currentFanState;
    SpanCharacteristic *targetFanState;
    // SpanCharacteristic *rotationDirection;
    SpanCharacteristic *rotationSpeed;
    SpanCharacteristic *swingMode;
    // SpanCharacteristic *lockPhysicalControls;

    Honeywell_Fan_Remote(const char *fanName) : Service::Fan() {
        try {
            name = new Characteristic::Name(fanName);
            active = new Characteristic::Active(0);
            // TODO: Figure out these characteristics
            // currentFanState = new Characteristic::CurrentFanState(0);
            // targetFanState = new Characteristic::TargetFanState(0);
            swingMode = new Characteristic::SwingMode(0);
            rotationSpeed = new Characteristic::RotationSpeed(0);

            Serial.printf("Configured Fan: %s\n", fanName);
        } catch (const std::exception& e) {
            Serial.println(e.what());
        }
    }

    bool update() override {
        if (active->updated()) {
            Serial.printf("Active: %d\n", active->getVal());
            IrSender.sendPulseDistanceWidth(38, 1300, 400, 1300, 400, 400, 1250, 0x10D, 11, PROTOCOL_IS_LSB_FIRST, 0,0);
        }
        if (currentFanState->updated()) {
            Serial.printf("Current Fan State: %d\n", currentFanState->getVal());
        }
        if (targetFanState->updated()) {
            Serial.printf("Target Fan State: %d\n", targetFanState->getVal());
        }
        if (swingMode->updated()) {
            Serial.printf("Swing Mode: %d\n", swingMode->getVal());
            IrSender.sendPulseDistanceWidth(38, 1350, 350, 1300, 400, 450, 1200, 0x61D, 11, PROTOCOL_IS_LSB_FIRST, 0,0);
        }

        // TODO: Implement fan speed down command, this will consist of sending the command 4 times.
        if (rotationSpeed->updated()) {
            // TODO: Message needs to be tweaked as the ir command increments speed and then loops back to 0
            Serial.printf("Rotation Speed: %d\n", rotationSpeed->getVal());
            IrSender.sendPulseDistanceWidth(38, 1300, 400, 1300, 350, 450, 1250, 0x31D, 11, PROTOCOL_IS_LSB_FIRST, 0,0);
        }
        return(true);
    }
};