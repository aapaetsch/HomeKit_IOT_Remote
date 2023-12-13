<h1>Homekit IOT Remote</h1>

- An IOT IR Remote with additional smart capabilities
- Setup as a homekit bridge with the following devices:
    - Television_Remote: For controlling a TCL Google TV as well as Edifier Speakers
        - Control through homekit app or through the remote widget in apple control center
        - Speaker volume only controllable through remote widget currently (volume rocker)
    - Speaker Remote: Controlling additional functions of the Edifier Speakers
        - Power
        - Switch to Opt Input
        - Switch to BT Input
        - Enable BT Pairing Mode
        - Volume (Currently not supported in official home app)
    - Climate Sensors: Local climate sensors 
        - Temperature (still needs to be connected to sensor itself)
        - Humidity (sensor not connected yet)
    - Fan: Basic Honeywell IR Tower Fan
        - Speed
        - Osc
        - Power

<h2>Libraries</h2>

- [HomeSpan](https://github.com/HomeSpan/HomeSpan): Arduino Library to connect esp32 to homekit
- Arduino.h
- IRremote.hpp
- Wire.h
- Adafruit_GFX.h
- Adafruit_SSD1305.h 

<h2>Parts List</h2>

- ESP32
- 1 x TCA9548A I2C Multiplexer
- 1 x SSD1306 OLED Display (I2C Protocol)
- 1 x AM2320 Temperature/Humidity Sensor (I2C Protocol)
- 2 x IR LED diodes
- 1 x IR Reciever
- 1 x Rotary Encoder + push button
- - 1 x 1K Resistor
- 1 x 47r Resistor
- 1 x 20r Resistor

<!-- // TODO... -->
<!-- - X x 4pin push button (todo: get exact measurements) -->
<!-- - 1 x Green LED -->
<!-- - 1 X Yellow LED -->
<!-- - 1 X Red LED  -->
<!-- <h2>Schematics<h2> -->
<!-- <h2>PCB</h2> -->
<!-- <h2>Pairing with Google Assistant<h2>
