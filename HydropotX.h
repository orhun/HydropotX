#define START_MSG "HydropotX v1.0"
#define TICK_RATE 5 * 1000
#define MOTOR_DELAY 20 * 60
#define PH_VALUE 7.0
#define EC_VALUE 0.17
/*
 * pH calibration values:
 * y1 = 5.5, y2 = 7.4
 * x1 = 10.11, x2 = 3.51
 * m = 7.4 - 5.5 / 3.51 - 10.11 = -0.28
 * b = 5.5 - (-0.28 x 10.11) = 8.3
 */
/*
 * m = (y2 - y1) / (x2 - x1)
 * b = y - mx
 */
#define PH_CONST_M -0.28
#define PH_CONST_B 8.3
/* Hana      [USA]        PPM converion:   0.50 */
/* Eutech    [EU]         PPM conversion:  0.64 */
/* Tranchen  [Australia]  PPM conversion:  0.70 */
#define PPM_CONV 0.5
/* Resistance of the powering pins */
#define EC_RA 25
/* Must be greater than 300 ohms */
#define EC_R1 1000
/* Constant for EC measurements */
#define EC_CONST 2.11
/* Temperature compensation. */
/* 0.019 is generally considered as the standard for plant nutrients. */
#define TEMP_COMP 0.019
/* Pins */
#define PH_PIN A2
#define EC_PIN A0
#define EC_GND A1
#define EC_VCC A4
#define TEMP_DATA_PIN 10
#define TEMP_VCC_PIN 8
#define TEMP_GND_PIN 9
#define MOTOR1_PIN 2
#define MOTOR2_PIN 3
