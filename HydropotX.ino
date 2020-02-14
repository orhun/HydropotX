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

        float getPh() {
            return values.ph;
        }

        float getEc() {
            return values.ec;
        }
};

enum Motor {
    PH_MOTOR,
    EC_MOTOR
};

class Motors {
    private:
        HCMotor hcMotor;
        void reset(Motor motor) {
            hcMotor.OnTime(motor, 10);
            delay(7500);
            hcMotor.OnTime(motor, 0);
            delay(500);
        }
    public:
        int delayTime = 0;
        void init() {
            hcMotor.Init();
            hcMotor.attach(PH_MOTOR, DCMOTOR, MOTOR1_PIN);
            hcMotor.DutyCycle(PH_MOTOR, 10);
            reset(PH_MOTOR);
            hcMotor.attach(EC_MOTOR, DCMOTOR, MOTOR2_PIN);
            hcMotor.DutyCycle(EC_MOTOR, 10);
            reset(EC_MOTOR);
        }

        bool start(Motor motor) {
            hcMotor.OnTime(motor, 10);
            delay(500);
            hcMotor.OnTime(motor, 0);
            return true;
        }
};

static Sensors sensors;
static Motors motors;

void setup() {
    Serial.begin(9600);
    Serial.println(START_MSG);
    Serial.println("Initializing...");
    sensors.init();
    motors.init();
    Serial.println("Reading the sensor values with a 5s interval...");
}

void loop() {
    if (sensors.read()) {
        sensors.print();
        if (motors.delayTime <= 0) {
            if (sensors.getEc() < EC_VALUE) {
                Serial.print("|   +EC    |   ");
                Serial.print(sensors.getEc());
                Serial.print("   <   ");
                Serial.print(EC_VALUE);
                Serial.println("   |");
                motors.start(EC_MOTOR);
            }
            if (sensors.getPh() > PH_VALUE) {
                Serial.print("|   -pH    |   ");
                Serial.print(sensors.getPh());
                Serial.print("   >   ");
                Serial.print(PH_VALUE);
                Serial.println("   |");
                motors.start(PH_MOTOR);
            }
            motors.delayTime = MOTOR_DELAY;
        }
    }
    if (motors.delayTime > 0) {
        motors.delayTime -= TICK_RATE / 1000;
    }
    delay(TICK_RATE);
}