/* FILE:    HCMotor_DC_Motor_With H_Bridge_Example
   DATE:    09/07/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

   
This example uses the library to control a DC motor via a potentiometer connected
analogue pin A0. With this example the motor is connected to the Arduino via a standard 
H-Bridge driver module such as HCMODU0033 or HCARDU0013 to allow the motor to be driven in 
both forward and reverse directions. 

Do not connect the motor directly to your Arduino's digital pins as you may damage your 
Arduino.

Note about driving more than one motor:
By default this library can drive up to 4 motors. However this can be increased by 
editing the following line in the libraries HCMotor.h file:

#define MAXMOTORS 4 <-- change to match the number of motors you require.

If you are using less than 4 motors and your sketch is processor intensive you may 
also want to reduce this value to match the number of motors you have attached as this 
will free up processing cycles for your main loop.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/


/* Include the library */
#include "HCMotor.h"

/* Pins used to drive the motors */
#define MOTOR_PINA 8 //For HCMODU0033 connect to A-IA, for HCARDU0013 connect to IN1
#define MOTOR_PINB 9 //For HCMODU0033 connect to A-IB, for HCARDU0013 connect to IN2

/* Set the analogue pin the potentiometer will be connected to. */
#define POT_PIN A0

/* Set a dead area at the centre of the pot where it crosses from forward to reverse */
#define DEADZONE 20 

/* The analogue pin will return values between 0 and 1024 so divide this up between 
   forward and reverse */
#define POT_REV_MIN 0
#define POT_REV_MAX (512 - DEADZONE)
#define POT_FWD_MIN (512 + DEADZONE)
#define POT_FWD_MAX 1024


/* Create an instance of the library */
HCMotor HCMotor;


void setup() 
{

  /* Initialise the library */
  HCMotor.Init();

  /* Attach motor 0 to digital pins 8 & 9. The first parameter specifies the 
     motor number, the second is the motor type, and the third and forth are the 
     digital pins that will control the motor */
  HCMotor.attach(0, DCMOTOR_H_BRIDGE, MOTOR_PINA, MOTOR_PINB);

  /* Set the duty cycle of the PWM signal in 100uS increments. 
     Here 100 x 100uS = 1mS duty cycle. */
  HCMotor.DutyCycle(0, 100);

}

void loop() 
{
  int Speed, Pot;

  /* Read the analogue pin to determine the position of the pot. */ 
  Pot = analogRead(POT_PIN);

  /* Is the pot in the reverse position ? */
  if (Pot >= POT_REV_MIN && Pot <= POT_REV_MAX)
  {
    HCMotor.Direction(0, REVERSE);
    Speed = map(Pot, POT_REV_MIN, POT_REV_MAX, 100, 0);

  /* Is the pot in the forward position ? */
  }else if (Pot >= POT_FWD_MIN && Pot <= POT_FWD_MAX)
  {
    HCMotor.Direction(0, FORWARD);
    Speed = map(Pot, POT_FWD_MIN, POT_FWD_MAX, 0, 100);

  /* Is the pot in the dead zone ? */
  }else
  {
    Speed = 0;
  }

  /* Set the on time of the duty cycle to match the position of the pot. */
  HCMotor.OnTime(0, Speed);

}
