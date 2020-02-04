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
#include <OneWire.h>
#include <DallasTemperature.h>

#define PH_PIN A2
#define PH_CONST_M -0.28 /* m = (y2 - y1) / (x2 - x1) */
#define PH_CONST_B 8.3   /* b = y - mx */
#define MOTOR1_PIN 2
#define MOTOR2_PIN 3
/* Hana      [USA]        PPM converion:   0.5  */
/* Eutech    [EU]         PPM conversion:  0.64 */
/* Tranchen  [Australia]  PPM conversion:  0.7  */
#define PPM_CONV 0.5
#define EC_RA 25      /* Resistance of the powering pins */
#define EC_R1 1000    /* Must be greater than 300 ohms */
#define EC_CONST 1.76 /* Constant for EC measurements */
#define EC_PIN A0
#define EC_GND A1
#define EC_VCC A4
/* Temperature compensation. */
/* 0.019 is generally considered as the standard for plant nutrients. */
#define TEMP_COMP 0.019
#define TEMP_DATA_PIN 10
#define TEMP_VCC_PIN 8
#define TEMP_GND_PIN 9

typedef struct {
    float ec;
    float rc;
    float ph;
    float ppm;
    float temp;
} SensorValues;

static OneWire oneWire(TEMP_DATA_PIN);
static DallasTemperature tempSensor(&oneWire);
static SensorValues sensorValues;

DFRobot_PH phSensor;
HCMotor HCMotor;

void setup() {
    Serial.begin(9600);
    Serial.println("START");
    pinMode(TEMP_GND_PIN, OUTPUT);
    pinMode(TEMP_VCC_PIN, OUTPUT);
    pinMode(EC_PIN, INPUT);
    digitalWrite(TEMP_GND_PIN, LOW);
    digitalWrite(TEMP_VCC_PIN, HIGH);
    digitalWrite(EC_GND, LOW);
    delay(500);
    tempSensor.begin();
    phSensor.begin();
    HCMotor.Init();
    HCMotor.attach(0, DCMOTOR, MOTOR1_PIN);
    HCMotor.attach(1, DCMOTOR, MOTOR2_PIN);
    HCMotor.DutyCycle(0, 100);
    HCMotor.DutyCycle(1, 100);
}

void loop() {
    /*HCMotor.OnTime(0, 30);
    delay(1000);
    HCMotor.OnTime(0, 0);
    delay(1000);
    HCMotor.OnTime(1, 40);
    delay(1000);
    HCMotor.OnTime(1, 0);
    delay(10000);*/
    sensorValues = readSensorValues();
    Serial.print("pH: ");
    Serial.print(sensorValues.ph);
    Serial.print(" EC: ");
    Serial.print(sensorValues.ec);
    Serial.print(" Siemens ");
    Serial.print(sensorValues.ppm);
    Serial.print(" ppm ");
    Serial.print(sensorValues.temp);
    Serial.println(" *C ");
    delay(5000);
}

SensorValues readSensorValues() {
    /* Read the temperature value from sensor. */
    tempSensor.requestTemperatures();
    float temperature = tempSensor.getTempCByIndex(0);
    /* Read the pH value from sensor. */
    float voltage = analogRead(PH_PIN) / 1024.0 * 5000;
    float phValue = phSensor.readPH(voltage, temperature) * PH_CONST_M + PH_CONST_B;
    phSensor.calibration(voltage, temperature);
    /* Estimate the resistance of the liquid. */
    pinMode(EC_VCC, OUTPUT);
    pinMode(EC_GND, OUTPUT);
    digitalWrite(EC_VCC, HIGH);
    float ecRaw = analogRead(EC_PIN);
    ecRaw = analogRead(EC_PIN);
    digitalWrite(EC_VCC, LOW);
    pinMode(EC_VCC, INPUT);
    pinMode(EC_GND, INPUT);
    /* Convert raw values to EC and compensate for the temperature. */
    float voltageDrop = (5 * ecRaw) / 1024.0;
    float rcValue = ((voltageDrop * (EC_R1 + EC_RA)) / (5 - voltageDrop)) - EC_RA;
    float ecValue  =  (1000 / (rcValue * EC_CONST)) / (1 + TEMP_COMP * (temperature - 25.0));
    float ppm = ecValue * PPM_CONV * 1000;
    return {.ec = ecValue, .rc = rcValue, .ph = phValue, .ppm = ppm, .temp = temperature};
}