# RCMv2
https://github.com/rcmgames/RCMv2

Work on creating AutoTuneJMotorCompStandard, a class for tuning automatically finding JMotorCompStandard motor calibration values.

To use, open a serial monitor then enable the robot. No other inputs do anything. After a couple minutes the calibration should finish.

### Open in VSCode (using [PlatformIO](https://platformio.org/platformio-ide)), or use the Arduino IDE.

### Driver Station: https://github.com/rcmgames/RCMDS-new (recommended)

### [Driver station config file "byte test"](https://github.com/RCMgames/RCMDS-new/blob/main/rcmds/data/byte%20test.txt)

### Libraries used:
* [JMotor](https://github.com/joshua-8/JMotor) library for motor control
* [ESP32_easy_wifi_data](https://github.com/joshua-8/ESP32_easy_wifi_data) as optional method of communication
