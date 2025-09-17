//   This program is template code for programming small esp32 powered wifi controlled robots.
//   https://github.com/rcmgames/RCMv2
//   for information see this page: https://github.com/RCMgames

/**
uncomment one of the following lines depending on which hardware you have
Remember to also choose the "environment" for your microcontroller in PlatformIO
*/
// #define RCM_HARDWARE_VERSION RCM_ORIGINAL // versions 1, 2, 3, and 3.1 of the original RCM hardware // https://github.com/RCMgames/RCM_hardware_documentation_and_user_guide
// #define RCM_HARDWARE_VERSION RCM_4_V1 // version 1 of the RCM 4 // https://github.com/RCMgames/RCM-Hardware-V4
#define RCM_HARDWARE_VERSION RCM_BYTE_V2 // version 2 of the RCM BYTE // https://github.com/RCMgames/RCM-Hardware-BYTE
// #define RCM_HARDWARE_VERSION RCM_NIBBLE_V1 // version 1 of the RCM Nibble // https://github.com/RCMgames/RCM-Hardware-Nibble
// #define RCM_HARDWARE_VERSION RCM_D1_V1 // version 1 of the RCM D1 // https://github.com/RCMgames/RCM-Hardware-D1

/**
uncomment one of the following lines depending on which communication method you want to use
*/
#define RCM_COMM_METHOD RCM_COMM_EWD // use the normal communication method for RCM robots
// #define RCM_COMM_METHOD RCM_COMM_ROS // use the ROS communication method

#include "rcm.h" //defines pins

// set up motors and anything else you need here
// See this page for how to set up servos and motors for each type of RCM board:
// https://github.com/RCMgames/useful-code/tree/main/boards
// See this page for information about how to set up a robot's drivetrain using the JMotor library
// https://github.com/joshua-8/JMotor/wiki/How-to-set-up-a-drivetrain
// all the servo drivers
JMotorDriverEsp32Servo servo1 = JMotorDriverEsp32Servo(port1);
JMotorDriverEsp32Servo servo2 = JMotorDriverEsp32Servo(port2);
JMotorDriverEsp32Servo servo3 = JMotorDriverEsp32Servo(port3);
JMotorDriverEsp32Servo servo4 = JMotorDriverEsp32Servo(port4);
JMotorDriverEsp32Servo servo5 = JMotorDriverEsp32Servo(port5);
JMotorDriverEsp32Servo servo6 = JMotorDriverEsp32Servo(port6);
JMotorDriverEsp32Servo servo7 = JMotorDriverEsp32Servo(port7);
JMotorDriverEsp32Servo servo8 = JMotorDriverEsp32Servo(port8);

// all the motor drivers
JMotorDriverTMC7300 motorA = JMotorDriverTMC7300(portA);
JMotorDriverTMC7300 motorB = JMotorDriverTMC7300(portB);
JMotorDriverTMC7300 motorC = JMotorDriverTMC7300(portC);
JMotorDriverTMC7300 motorD = JMotorDriverTMC7300(portD);
JMotorDriverTMC7300 motorE = JMotorDriverTMC7300(portE);
JMotorDriverTMC7300 motorF = JMotorDriverTMC7300(portF);
JMotorDriverTMC7300 motorG = JMotorDriverTMC7300(portG);
JMotorDriverTMC7300 motorH = JMotorDriverTMC7300(portH);

// variables for the drivers
float servo1Val = 0;
float servo2Val = 0;
float servo3Val = 0;
float servo4Val = 0;
float servo5Val = 0;
float servo6Val = 0;
float servo7Val = 0;
float servo8Val = 0;

float motorAVal = 0;
float motorBVal = 0;
float motorCVal = 0;
float motorDVal = 0;
float motorEVal = 0;
float motorFVal = 0;
float motorGVal = 0;
float motorHVal = 0;

void Enabled()
{
    // code to run while enabled, put your main code here
    // set all the motor drivers (you can put this in Enabled())
    servo1.set(servo1Val);
    servo2.set(servo2Val);
    servo3.set(servo3Val);
    servo4.set(servo4Val);
    servo5.set(servo5Val);
    servo6.set(servo6Val);
    servo7.set(servo7Val);
    servo8.set(servo8Val);

    motorA.set(motorAVal);
    motorB.set(motorBVal);
    motorC.set(motorCVal);
    motorD.set(motorDVal);
    motorE.set(motorEVal);
    motorF.set(motorFVal);
    motorG.set(motorGVal);
    motorH.set(motorHVal);

    printDebugInfoForMotorDriver(motorA.ic);
}

void printDebugInfoForMotorDriver(TMC7300IC& ic)
{
    // see table 5.3.3 https://www.analog.com/media/en/technical-documentation/data-sheets/TMC7300_datasheet_rev1.08.pdf
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint < 100) {
        return; // don't print too fast to read
    }
    lastPrint = millis();
    Serial.flush();
    delay(10);
    uint32_t drvErrorField = ic.readField(TMC7300_DRV_ERR);
    Serial.print(drvErrorField);
    uint32_t value;
    Serial.flush();
    delay(10);
    if (ic.readRegister(_TMC7300_DRVSTATUS, value)) {
        Serial.print(" DRVSTATUS: ");
        Serial.print(value & 0b1111111111, BIN);
    }
    Serial.println();
}

void Enable()
{
    // turn on outputs
    // enable all the motor drivers (you can put this in Enable())
    servo1.enable();
    servo2.enable();
    servo3.enable();
    servo4.enable();
    servo5.enable();
    servo6.enable();
    servo7.enable();
    servo8.enable();

    motorA.enable();
    motorB.enable();
    motorC.enable();
    motorD.enable();
    motorE.enable();
    motorF.enable();
    motorG.enable();
    motorH.enable();
}

void Disable()
{
    // turn off outputs
    // disable all the motor drivers (you can put this in Disable())
    servo1.disable();
    servo2.disable();
    servo3.disable();
    servo4.disable();
    servo5.disable();
    servo6.disable();
    servo7.disable();
    servo8.disable();

    motorA.disable();
    motorB.disable();
    motorC.disable();
    motorD.disable();
    motorE.disable();
    motorF.disable();
    motorG.disable();
    motorH.disable();
}

void PowerOn()
{
    // runs once on robot startup, set pin modes and use begin() if applicable here
    TMC7300_AB.writeField(TMC7300_IRUN, 12); // current limit max 31
    // TMC7300_CD.writeField(TMC7300_IRUN, 12);
    // TMC7300_EF.writeField(TMC7300_IRUN, 12);
    // TMC7300_GH.writeField(TMC7300_IRUN, 12);
}

void Always()
{
    // always runs if void loop is running, JMotor run() functions should be put here
    // (but only the "top level", for example if you call drivetrainController.run() you shouldn't also call leftMotorController.run())

    delay(1);
}

#if RCM_COMM_METHOD == RCM_COMM_EWD
void WifiDataToParse()
{
    enabled = EWD::recvBl();
    // add data to read here: (EWD::recvBl, EWD::recvBy, EWD::recvIn, EWD::recvFl)(boolean, byte, int, float)
    // receive values for all the variables (you can put this in WifiDataToParse())
    motorAVal = EWD::recvFl();
    motorBVal = EWD::recvFl();
    motorCVal = EWD::recvFl();
    motorDVal = EWD::recvFl();
    motorEVal = EWD::recvFl();
    motorFVal = EWD::recvFl();
    motorGVal = EWD::recvFl();
    motorHVal = EWD::recvFl();

    servo1Val = EWD::recvFl();
    servo2Val = EWD::recvFl();
    servo3Val = EWD::recvFl();
    servo4Val = EWD::recvFl();
    servo5Val = EWD::recvFl();
    servo6Val = EWD::recvFl();
    servo7Val = EWD::recvFl();
    servo8Val = EWD::recvFl();
}
void WifiDataToSend()
{
    EWD::sendFl(voltageComp.getSupplyVoltage());
    // add data to send here: (EWD::sendBl(), EWD::sendBy(), EWD::sendIn(), EWD::sendFl())(boolean, byte, int, float)
}

void configWifi()
{
    EWD::mode = EWD::Mode::connectToNetwork;
    EWD::routerName = "router";
    EWD::routerPassword = "password";
    EWD::routerPort = 25210;

    // EWD::mode = EWD::Mode::createAP;
    // EWD::APName = "rcm0";
    // EWD::APPassword = "rcmPassword";
    // EWD::APPort = 25210;
}
#elif RCM_COMM_METHOD == RCM_COMM_ROS ////////////// ignore everything below this line unless you're using ROS mode /////////////////////////////////////////////
void ROSWifiSettings()
{
    // SSID, password, IP, port (on a computer run: sudo docker run -it --rm --net=host microros/micro-ros-agent:iron udp4 --port 8888 )
    set_microros_wifi_transports("router", "password", "10.25.21.1", 8888); // doesn't complete until it connects to the wifi network
    nodeName = "rcm_robot";
    // numSubscribers = 10; //change max number of subscribers
}

#include <example_interfaces/msg/bool.h>
#include <std_msgs/msg/byte.h>
#include <std_msgs/msg/float32.h>
#include <std_msgs/msg/int32.h>
// and lots of other message types are available (see file available_ros2_types)
// #include <geometry_msgs/msg/twist.h>

// declare publishers
declarePub(battery, std_msgs__msg__Float32);

// // declare subscribers and write callback functions
// declareSubAndCallback(cmd_vel, geometry_msgs__msg__Twist);
// velCmd.x = cmd_velMsg->linear.x;
// velCmd.y = cmd_velMsg->linear.y;
// velCmd.theta = cmd_velMsg->angular.z;
// } // end of callback

void ROSbegin()
{
    // create publishers
    createPublisher(battery, std_msgs__msg__Float32, "/rcm/battery");
    batteryMsg.data = 0;

    // add subscribers
    // addSub(cmd_vel, geometry_msgs__msg__Twist, "/cmd_vel");
}

void ROSrun()
{
    rosSpin(1);
    // you can add more publishers here
    batteryMsg.data = voltageComp.getSupplyVoltage();
    publish(battery);
}
#endif

#include "rcmutil.h"
