/* HydropotX
 *
 * upload:  arduino --upload HydropotX.ino --port /dev/ttyUSB*
 * monitor: stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 * arduino --upload HydropotX.ino --port /dev/ttyUSB* && stty -F /dev/ttyUSB* 9600 raw -clocal -echo && cat /dev/ttyUSB*
 *
 */

void setup() {
    Serial.begin(9600);  
    Serial.println("START");
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    Serial.println("HIGH");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    Serial.println("LOW");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}