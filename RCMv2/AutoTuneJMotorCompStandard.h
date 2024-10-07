#ifndef AUTO_TUNE_JMOTOR_COMP_STANDARD_H
#define AUTO_TUNE_JMOTOR_COMP_STANDARD_H
#include <JMotor.h>
class AutoTuneJMotorCompStandard {
public:
    JEncoder& encoder;
    JMotorDriver& motor;
    JVoltageCompensator& voltComp;
    JMotorCompStandardConfig& config;

    int state = -1;

    unsigned long lastTime = 0;

    float voltage;

    static const int numStartTrials = 5;
    float startSpeeds[numStartTrials];
    float startVolts[numStartTrials];
    float stopVolts[numStartTrials];
    int startTrialI = 0;

    static const int numHighReadings = 15;
    float motorHighVoltage[numHighReadings];
    float motorHighSpeed[numHighReadings];
    int highTrialI = 0;

    AutoTuneJMotorCompStandard(JEncoder& _encoder, JMotorDriver& _motor, JVoltageCompensator& _voltComp, JMotorCompStandardConfig& _config)
        : encoder(_encoder)
        , motor(_motor)
        , voltComp(_voltComp)
        , config(_config)
    {
        state = -1;
    }
    void start()
    {
        state = 0;
        motor.enable();
    }
    void stop()
    {
        state = -1;
        motor.set(0);
        motor.disable();
    }
    void tune()
    {
        encoder.run();
        if (state != -1) {
            Serial.printf("slow trial %d/%d state=%d voltage=%f speed=%f position=%f \n", startTrialI + 1, numStartTrials, state, voltage, encoder.getVel(), encoder.getPos());
        }
        switch (state) {
        case 0: { // set up initial values
            startTrialI = -1;

            motor.set(0);
            state = 1;
            lastTime = millis();
            break;
        }
        case 1: { // give the motor time to stop (if it was moving before this process started)
            motor.set(0);
            if (millis() - lastTime > 500) {
                encoder.zeroCounter();
                state = 2;
                lastTime = millis();
                startTrialI++;
            }
            break;
        }
        case 2: { // find what voltage starts the motor
            voltage = (millis() - lastTime) / 3000.0; // slowly ramp voltage
            motor.set(voltComp.adjust(voltage, motor.getMaxRange()));
            if (encoder.getVel() > 0 && encoder.getCounter() > 10 /*TODO: REMOVE HARD-CODED NUMBER*/) { // if it's started turning and moved enough it's clear it's not just noise
                startVolts[startTrialI] = voltage;
                lastTime = millis();
                state = 3;
            }
            break;
        }
        case 3: { // give the motor time to settle at the start speed
            if (millis() - lastTime > 1000) {
                lastTime = millis();
                startSpeeds[startTrialI] = encoder.getVel();
                state = 4;
            }
            break;
        }
        case 4: { // find what voltage the motor stops at
            voltage = startVolts[startTrialI] - (millis() - lastTime) / 5000.0;
            if (voltage < 0) {
                voltage = 0;
            }
            if (voltage > voltComp.getSupplyVoltage()) {
                voltage = voltComp.getSupplyVoltage();
            }
            motor.set(voltComp.adjust(voltage, motor.getMaxRange()));
            if (encoder.getVel() == 0) {
                // TODO: how detect stopSpeed?
                stopVolts[startTrialI] = voltage;
                lastTime = millis();
                if ((startTrialI + 1) >= numStartTrials) { // done with slow trials, do high voltage measurements
                    state = 5;
                    lastTime = millis();
                } else { // do another trial of when the motor starts
                    state = 1;
                    lastTime = millis();
                }
            }
            break;
        }
        case 5: { // spin up motor for fast trial
            motor.set(1);
            if (millis() - lastTime > 1000) { // enough time to spin up
                state = 6;
                lastTime = millis();
                highTrialI = 0;
            }
            break;
        }
        case 6: { // take readings of voltage and speed at fast speed
            if (millis() - lastTime > 250) {
                lastTime = millis();
                motorHighVoltage[highTrialI] = voltComp.getSupplyVoltage();
                motorHighSpeed[highTrialI] = encoder.getVel();
                highTrialI++;
                if (highTrialI >= numHighReadings) {
                    motor.set(0);
                    motor.disable();
                    state = 100;
                }
            }
            break;
        }
        case 100: { // process data
            motor.set(0);
            motor.disable();
            config.motor_high_speed = 0;
            config.motor_high_voltage = 0;
            config.motor_start_speed = 0;
            config.motor_start_voltage = 0;
            config.motor_stop_speed = 0;
            config.motor_stop_voltage = 0;
            config.start_boost_time = 0; // TODO: HOW MEASURE THIS

            for (int i = 0; i < numStartTrials; i++) {
                config.motor_start_speed += startSpeeds[i];
                config.motor_start_voltage += startVolts[i];
                config.motor_stop_voltage += stopVolts[i];
            }
            config.motor_start_speed /= numStartTrials;
            config.motor_start_voltage /= numStartTrials;
            config.motor_stop_voltage /= numStartTrials;

            for (int i = 0; i < numHighReadings; i++) {
                config.motor_high_speed += motorHighSpeed[i];
                config.motor_high_voltage += motorHighVoltage[i];
            }
            config.motor_high_speed /= numHighReadings;
            config.motor_high_voltage /= numHighReadings;

            Serial.printf("motor_start_speed=%f\n motor_start_voltage=%f\n motor_stop_voltage=%f\n motor_high_speed=%f\n motor_high_voltage=%f\n", config.motor_start_speed, config.motor_start_voltage, config.motor_stop_voltage, config.motor_high_speed, config.motor_high_voltage);
            state = -1;

            break;
        }
        default:
            break;
        }
    }
};
#endif // AUTO_TUNE_JMOTOR_COMP_STANDARD_H
