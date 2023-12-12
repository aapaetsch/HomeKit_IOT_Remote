#include <Arduino.h>
#include <HomeSpan.h>
#include <IRremote.hpp>
#include <remoteCommands.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LED.h>
#include <Television_Remote.h>
#include <Push_Button.h>
#include <Honeywell_Fan_Remote.h>

#include <IR_Remote_Button.h>

#define BLINK_LED 2
#define REMOTE_LED_PIN 17
#define BAUD_RATE 115200

// Pairing Code and QRID, change these to your own
#define PAIRING_CODE "11122333"
#define QRID "111-22-333"

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

void setup() {
    // Serial Setup
    Serial.begin(BAUD_RATE);

    // Pin Setup
    pinMode(BLINK_LED, OUTPUT);
    pinMode(REMOTE_LED_PIN, OUTPUT);

    // HomeSpan Setup 
    homeSpan.setPairingCode(PAIRING_CODE);
    homeSpan.setQRID(QRID);
    homeSpan.begin(Category::Bridges, "HomeSpan IR Remote");

    // HomeSpan Accessory Setup Start
    // blank accessory as first accessory for bridge
    new SpanAccessory();
        new Service::AccessoryInformation();
        new Characteristic::Identify();

    // Tv Controls
    new SpanAccessory();
        new Service::AccessoryInformation();
            new Characteristic::Name("Living Room TV");
            new Characteristic::Manufacturer("TCL");
            new Characteristic::Model("50Q550G-CA Google TV (2021)");
            new Characteristic::Identify();
        new Television_Remote("Living Room TV", 
            {
                // Note: Amazon Prime and Youtube setup as they are simple button presses and can be driven by a singular remote command
                {"Computer", 1}, 
                {"Amazon Prime", 4},
                {"YouTube", 5},
                // {"HDMI 2", 2}, 
                // {"HDMI 3", 3}, 
            });

    // Speaker Controls TODO: Simplify
    new SpanAccessory();
        new Service::AccessoryInformation();
            new Characteristic::Name("Living Room Speaker Power");
            new Characteristic::Manufacturer("Edifier");
            new Characteristic::Model("R1850DB");
            new Characteristic::Identify();
        new Edifier_Remote_Button("Speaker Power", EDIFIER_SPEAKER_COMMANDS::SPEAKER_POWER, 500);

    new SpanAccessory();
        new Service::AccessoryInformation();
            new Characteristic::Name("Living Room Speaker Computer Input");
            new Characteristic::Manufacturer("Edifier");
            new Characteristic::Model("R1850DB");
            new Characteristic::Identify();
        new Edifier_Remote_Button("Speaker Computer Input", EDIFIER_SPEAKER_COMMANDS::INPUT_OPT, 500);

    new SpanAccessory();
        new Service::AccessoryInformation();
            new Characteristic::Name("Living Room Speaker Bluetooth Input");
            new Characteristic::Manufacturer("Edifier");
            new Characteristic::Model("R1850DB");
            new Characteristic::Identify();
        new Edifier_Remote_Button("Speaker Bluetooth Input", EDIFIER_SPEAKER_COMMANDS::INPUT_BT, 500);
    
    new SpanAccessory();
        new Service::AccessoryInformation();
            new Characteristic::Name("Living Room Speaker Volume Up");
            new Characteristic::Manufacturer("Edifier");
            new Characteristic::Model("R1850DB");
            new Characteristic::Identify();
        new Edifier_Remote_Button("Speaker Volume Up", EDIFIER_SPEAKER_COMMANDS::VOLUME_UP, 500);

    // new SpanAccessory();
    //     new Service::AccessoryInformation();
    //         new Characteristic::Identify();
    //     new Honeywell_Fan_Remote("Living Room Fan");
    // HomeSpan Accessory Setup End
}

void blinkLED(int pin, unsigned long delayTime) {
    if (currentMillis - previousMillis >= delayTime)
    {
        bool ledState = digitalRead(pin);
        digitalWrite(pin, !ledState);
    }
}

void loop() {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
        blinkLED(BLINK_LED, 1000);
        previousMillis = currentMillis;
    }
    homeSpan.poll();
}