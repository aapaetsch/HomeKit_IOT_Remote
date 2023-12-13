#include <Arduino.h>
#include <HomeSpan.h>

struct Temp_Sensor : Service::TemperatureSensor {
    SpanCharacteristic *name;
    SpanCharacteristic *currentTemperature;
    SpanCharacteristic *statusActive;

    Temp_Sensor(const char *sensorName) : Service::TemperatureSensor() {
        name = new Characteristic::Name(sensorName);
        currentTemperature = new Characteristic::CurrentTemperature();
        statusActive = new Characteristic::StatusActive();
    }
    
    void setIsActive(bool isActive) {
        statusActive->setVal(isActive);
    }

    void setTemperature(float temperature) {
        currentTemperature->setVal(temperature);
    }

    bool update() override {
        if (currentTemperature->updated()) {
            Serial.printf("Temperature: %f\n", currentTemperature->getVal());
        }
        if (statusActive->updated()) {
            Serial.printf("Status Active: %d\n", statusActive->getVal());
        }
        return(true);
    }
};

struct Humidity_Sensor : Service::HumiditySensor {
    SpanCharacteristic *name;
    SpanCharacteristic *currentHumidity;
    SpanCharacteristic *statusActive;

    Humidity_Sensor(const char *sensorName) : Service::HumiditySensor() {
        name = new Characteristic::Name(sensorName);
        currentHumidity = new Characteristic::CurrentRelativeHumidity();
        statusActive = new Characteristic::StatusActive();
    }

    void setIsActive(bool isActive) {
        statusActive->setVal(isActive);
    }

    void setHumidity(float humidity) {
        currentHumidity->setVal(humidity);
    }

    bool update() override {
        if (currentHumidity->updated()) {
            Serial.printf("Humidity: %f\n", currentHumidity->getVal());
        }
        if (statusActive->updated()) {
            Serial.printf("Status Active: %d\n", statusActive->getVal());
        }
        return(true);
    }
};
