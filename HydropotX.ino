/* HydropotX
 *
 * upload:  arduino --upload HydropotX.ino --port /dev/ttyUSB*
 * monitor: stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 * arduino --upload HydropotX.ino --port /dev/ttyUSB* && stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 * calibration values:
 * y1 = 5.5, y2 = 7.4
 * x1 = 10.11, x2 = 3.51
 * m = 7.4 - 5.5 / 3.51 - 10.11 = -0.28
 * b = 5.5 - (-0.28 x 10.11) = 8.3
 *
 */

#include "DFRobot_PH.h"
#include "HCMotor.h"
#include <EEPROM.h>

#define PH_PIN A0
#define MOTOR1_PIN 2
#define MOTOR2_PIN 3
#define CONST_M -0.28
#define CONST_B 8.3

float voltage, phValue, temperature;
DFRobot_PH ph;
HCMotor HCMotor;

void setup() {
    Serial.begin(9600);  
    Serial.println("START");
    ph.begin();
    HCMotor.Init();
    HCMotor.attach(0, DCMOTOR, MOTOR1_PIN);
    HCMotor.attach(1, DCMOTOR, MOTOR2_PIN);
    HCMotor.DutyCycle(0, 100);
    HCMotor.DutyCycle(1, 100);
}

void loop() {
    static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U) {
        timepoint = millis();
        temperature = readTemperature();
        voltage = analogRead(PH_PIN) / 1024.0 * 5000;
        phValue = ph.readPH(voltage, temperature) * CONST_M + CONST_B;
        Serial.print("Temperature: ");
        Serial.print(temperature, 1);
        Serial.print(", pH: ");
        Serial.println(phValue, 2);
    }
    ph.calibration(voltage, temperature);
    /*HCMotor.OnTime(0, 30);
    delay(1000);
    HCMotor.OnTime(0, 0);
    delay(1000);
    HCMotor.OnTime(1, 40);
    delay(1000);
    HCMotor.OnTime(1, 0);
    delay(10000);*/
}

float readTemperature() {
    return 25.0;
}
