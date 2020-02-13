/* HydropotX
 *
 * upload:  arduino --upload *.ino --port /dev/ttyUSB*
 * monitor: stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 * arduino --upload *.ino --port /dev/ttyUSB* && stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 */

#include "HydropotX.h"
#include "DFRobot_PH.h"
#include "HCMotor.h"
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class Sensors {
    private:
        typedef struct {
            float ec;
            float rc;
            float ph;
            float ppm;
            float temp;
        } Values;
        Values values;
        OneWire oneWire;
        DallasTemperature tempSensor;
        DFRobot_PH phSensor;
        bool firstValueRead;

    public:
        Sensors() {
            firstValueRead = false;
            oneWire = OneWire(TEMP_DATA_PIN);
            tempSensor = DallasTemperature(&oneWire);
        }

        void init() {
            pinMode(TEMP_GND_PIN, OUTPUT);
            pinMode(TEMP_VCC_PIN, OUTPUT);
            pinMode(EC_PIN, INPUT);
            digitalWrite(TEMP_GND_PIN, LOW);
            digitalWrite(TEMP_VCC_PIN, HIGH);
            digitalWrite(EC_GND, LOW);
            tempSensor.begin();
            phSensor.begin();
        }

        bool read() {
            tempSensor.requestTemperatures();
            values.temp = tempSensor.getTempCByIndex(0);
            /* Read the pH value from sensor. */
            float voltage = analogRead(PH_PIN) / 1024.0 * 5000;
            values.ph = phSensor.readPH(voltage, values.temp) * PH_CONST_M + PH_CONST_B;
            phSensor.calibration(voltage, values.temp);
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
            values.rc = ((voltageDrop * (EC_R1 + EC_RA)) / (5 - voltageDrop)) - EC_RA;
            values.ec  =  (1000 / (values.rc * EC_CONST)) / (1 + TEMP_COMP * (values.temp - 25.0));
            values.ppm = values.ec * PPM_CONV * 1000;
            if (!firstValueRead) {
                firstValueRead = true;
                return false;
            }
            return true;
        }

        void print() {
            Serial.print("| pH: ");
            Serial.print(values.ph);
            Serial.print(" | EC: ");
            Serial.print(values.ec);
            Serial.print(" | T: ");
            Serial.print(values.temp);
            Serial.println(" |");
        }
};

static Sensors sensors;
static HCMotor HCMotor;

void setup() {
    Serial.begin(9600);
    Serial.println(START_MSG);
    Serial.println("Reading the sensor values with a 5s interval...");
    sensors.init();
    HCMotor.Init();
    HCMotor.attach(0, DCMOTOR, MOTOR1_PIN);
    HCMotor.attach(1, DCMOTOR, MOTOR2_PIN);
    HCMotor.DutyCycle(0, 100);
    HCMotor.DutyCycle(1, 100);
}

void loop() {
    /*HCMotor.OnTime(0, 100);
    delay(10000);
    HCMotor.OnTime(0, 0);
    delay(2000);
    HCMotor.OnTime(1, 100);
    delay(10000);
    HCMotor.OnTime(1, 0);
    delay(2000);*/
    if (sensors.read()) {
        sensors.print();
    }
    delay(5000);
}