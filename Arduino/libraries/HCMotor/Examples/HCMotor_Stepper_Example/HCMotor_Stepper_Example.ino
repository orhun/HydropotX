/* FILE:    HCMotor_Stepper_Example
   DATE:    09/07/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

   
This example uses the library to control a stepper motor via a standard stepper driver 
module (with step/clock & direction inputs) using a potentiometer connected to
analogue pin A0. For suitable driver modules see items HCMODU0022 & HCMODU0068.

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
#define DIR_PIN 8 //Connect to drive modules 'direction' input.
#define CLK_PIN 9 //Connect to drive modules 'step' or 'CLK' input.

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
  //Serial.begin(9600);
  /* Initialise the library */
  HCMotor.Init();

  /* Attach motor 0 to digital pins 8 & 9. The first parameter specifies the 
     motor number, the second is the motor type, and the third and forth are the 
     digital pins that will control the motor */
  HCMotor.attach(0, STEPPER, CLK_PIN, DIR_PIN);

  /* Set the number of steps to continuous so the the motor is always turning whilst 
     not int he dead zone*/
  HCMotor.Steps(0,CONTINUOUS);
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
    Speed = map(Pot, POT_REV_MIN, POT_REV_MAX, 10, 1024);

  /* Is the pot in the forward position ? */
  }else if (Pot >= POT_FWD_MIN && Pot <= POT_FWD_MAX)
  {
    HCMotor.Direction(0, FORWARD);
    Speed = map(Pot, POT_FWD_MIN, POT_FWD_MAX, 1024, 10);

  /* Is the pot in the dead zone ? */
  }else
  {
    Speed = 0;
  }

  /* Set the duty cycle of the clock signal in 100uS increments */
  HCMotor.DutyCycle(0, Speed);

}
