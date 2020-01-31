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
#include <EEPROM.h>

#define PH_PIN A0
#define CONST_M -0.28
#define CONST_B 8.3

float voltage, phValue, temperature;
DFRobot_PH ph;

void setup() {
    Serial.begin(9600);  
    Serial.println("START");
    pinMode(LED_BUILTIN, OUTPUT);
    ph.begin();
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
    digitalWrite(LED_BUILTIN, HIGH);
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);
}

float readTemperature() {
    return 25.0;
}
