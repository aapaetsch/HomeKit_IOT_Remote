#include <HomeSpan.h>
struct Push_Button : Service::Switch {
    SpanCharacteristic *name;
    SpanCharacteristic *on;
    unsigned long delay;
    unsigned long previousMillis = 0;
    unsigned long currentMillis = 0;

    Push_Button(unsigned long debounceDelay = 50): Service::Switch() {
        name = new Characteristic::Name("Button");
        on = new Characteristic::On();
        delay = debounceDelay;
        currentMillis = millis();
    }

    void turnOff() {
        on->setVal(0);
        previousMillis = millis();
    }

    boolean update() override {
        try
        {
            currentMillis = millis();
            if (on->updated() && on->getVal() != on->getNewVal()) {
                if (currentMillis - previousMillis >= delay) {
                    Serial.printf("Button %s was pushed\n", name->getNewVal());
                    // if (on->getNewVal() == 1) {
                    //     Serial.printf("Turned on");
                    // } else {
                    //     Serial.printf("Turned off");
                    // }
                    previousMillis = currentMillis;
                } else {
                    Serial.printf("Button %s was pushed too soon\n", name->getNewVal());
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