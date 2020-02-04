/* FILE:    HCMotor.h
   DATE:    09/07/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

11/03/15 version 0.1: Original version  
   
Library header for driving DC and stepper motors.

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


#ifndef HCMotor_h
#define HCMotor_h

#include "Arduino.h"

/******************************************************************************
	CHANGE THIS VALUE TO MATCH THE NUMBER OF MOTORS YOU WISH TO ATTACH

Increasing this value will allow you to drive more motors. Reducing it will
increase the speed of your main loop 
/*****************************************************************************/	

#define MAXMOTORS 4

/*****************************************************************************/	




#define REVERSE false
#define FORWARD true
#define CONTINUOUS 0xFFFF

/* Supported Motor types */
enum MotorType
{
	DCMOTOR,
	DCMOTOR_H_BRIDGE,
	STEPPER
};


/* Structure used to store information about each attached motor type */
struct MotorInfo
{ 
   uint8_t MotorType;
   volatile uint8_t* PortA; 
   volatile uint8_t* PortB;
   volatile uint8_t* ActivePort;   
   uint8_t MaskA; 
   uint8_t MaskB;
   uint8_t ActiveMask; 
   uint8_t PinA;
   uint8_t PinB;
   unsigned int Counter = 0;
   unsigned int PulseLength = 0;
   unsigned int PulseHigh = 0;
   unsigned int Steps = 0;
   boolean Attached = false;
   boolean Direction = FORWARD;
}; 



/* Timer 2 clock prescaling values */
enum MotorT2PreScaller
{
	T2_CLK_DIV_0    = 1,
	T2_CLK_DIV_8    = 2,
	T2_CLK_DIV_32   = 3,
	T2_CLK_DIV_64   = 4,
    T2_CLK_DIV_128  = 5,
    T2_CLK_DIV_256  = 6,
    T2_CLK_DIV_1024 = 7
};


class HCMotor
{
	public:
	HCMotor(void);
	void Init(void);
	void attach(byte MotorNum, byte MotorType, byte Pin);
	void attach(byte MotorNum, byte MotorType, byte PinA, byte PinB);
	void detach(byte MotorNum);
	void DutyCycle(byte MotorNum, unsigned int Time);
	void OnTime(byte MotorNum, unsigned int Time);
	void Direction(byte MotorNum, boolean Direction);
	void Steps(byte MotorNum, unsigned int Steps);
	
	private:
	void _MotorTimer2(void);
	void _MotorTimer2Init(byte prescaler, byte compare);
};



#endif