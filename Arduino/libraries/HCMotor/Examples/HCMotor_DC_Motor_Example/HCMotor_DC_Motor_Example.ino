/* FILE:    HCMotor_DC_Motor_Example
   DATE:    09/07/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

   
This example uses the library to control a DC motor via a potentiometer connected
analogue pin A0. The motor should be connected to digital pin 7 on the Arduino 
via a suitable transistor/driver module. Do not connect the motor directly as you
may damage your Arduino.

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

/* Set the pin that will control the motor. Note that it doesn't have to be a PWM pin - 
   any digital pin will do! */
#define MOTOR_PIN 7

/* Set the analogue pin the potentiometer will be connected to. */
#define POT_PIN A0

/* Create an instance of the library */
HCMotor HCMotor;


void setup() 
{
  /* Initialise the library */
  HCMotor.Init();

  /* Attach motor 0 to digital pin 7. The first parameter specifies the 
     motor number, the second is the motor type, and the third is the 
     digital pin that will control the motor */
  HCMotor.attach(0, DCMOTOR, MOTOR_PIN);

  /* Set the duty cycle of the PWM signal in 100uS increments. 
     Here 100 x 100uS = 1mS duty cycle. */
  HCMotor.DutyCycle(0, 100);
}



void loop() 
{
  int Speed;

  /* Read the analogue pin to determine the position of the pot. The map 
     function takes this value which could be anywhere between 0 - 1024 
     and reduces it down to match the duty cycle range of 0 - 100 */ 
  Speed = map(analogRead(POT_PIN), 0, 1024, 0, 100);
  
  /* Set the on time of the duty cycle to match the position of the pot. */
  HCMotor.OnTime(0, Speed);
}
