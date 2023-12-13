// #define BLINK_LED 2
// #define REMOTE_LED_PIN 17

// #define BAUD_RATE 115200
// #define PAIRING_CODE "11122333"
// #define QRID "111-22-333"
// // put function declarations here:
// // int myFunction(int, int);

// unsigned long previousMillis = 0;
// unsigned long currentMillis = 0;
// void setup() {
//   // Serial Setup
//   Serial.begin(BAUD_RATE);

//   // Pin Setup
//   pinMode(BLINK_LED, OUTPUT);
//   pinMode(REMOTE_LED_PIN, OUTPUT);

//   // HomeSpan Setup 
//   homeSpan.setPairingCode(PAIRING_CODE);
//   homeSpan.setQRID(QRID);
//   homeSpan.begin(Category::Bridges, "HomeSpan IR Remote");

//   // HomeSpan Accessory Setup Start
//   new SpanAccessory();
//     new Service::AccessoryInformation();
//       new Characteristic::Identify();
  
//   // Test LED
//   new SpanAccessory();
//     new Service::AccessoryInformation();
//       new Characteristic::Identify();
//     new LED(REMOTE_LED_PIN);

//   // Tv Controls
//   new SpanAccessory();
//     new Service::AccessoryInformation();
//       new Characteristic::Identify();
//     new Television_Remote();
//   new SpanAccessory();
//     new Service::AccessoryInformation();
//       new Characteristic::Identify();
//     new Speaker_Remote();

//   // HomeSpan Accessory Setup End

// }

// void blinkLED(int pin, unsigned long delayTime) {
//   if (currentMillis - previousMillis >= delayTime)
//   {
//     bool ledState = digitalRead(pin);
//     digitalWrite(pin, !ledState);
    
//     if (!ledState) {
//       Serial.println("LED is on");
//     } else {
//       Serial.println("LED is off");
//     }
//   }
// }

// void loop() {
  
//   currentMillis = millis();
//   if (currentMillis - previousMillis >= 1000) {
//     blinkLED(BLINK_LED, 1000);
//     previousMillis = currentMillis;
//     Serial.println(currentMillis);
//   }
//   homeSpan.poll();
// }