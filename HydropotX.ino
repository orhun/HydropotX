/* HydropotX
 *
 * upload:  arduino --upload HydropotX.ino --port /dev/ttyUSB*
 * monitor: stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 * arduino --upload HydropotX.ino --port /dev/ttyUSB* && stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 */

#define PH_SENSOR 0
unsigned long int phAverage;
float b;
int buf[10], temp;

void setup() {
    Serial.begin(9600);  
    Serial.println("START");
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    for(int i = 0; i < 10; i++) {
        buf[i] = analogRead(PH_SENSOR);
        delay(10);
    }
    for(int i = 0; i < 9; i++) {
        for(int j = i + 1; j < 10; j++) {
            if(buf[i]>buf[j]){
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    phAverage = 0;
    for(int i = 2; i < 8; i++)
        phAverage += buf[i];
    float phValue = (float) phAverage * 5.0 / 1024 / 6;
    phValue = 3.5 * phValue;
    Serial.print("    pH:");
    Serial.print(phValue, 2);
    Serial.println(" ");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);
}