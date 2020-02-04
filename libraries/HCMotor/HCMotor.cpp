/* FILE:    HCMotor.cpp
   DATE:    09/07/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

11/03/15 version 0.1: Original version  
   
Library for driving DC and stepper motors.

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


#include "HCMotor.h"
#include <avr/interrupt.h>

MotorInfo Motor[MAXMOTORS];


/* Constructor to initialise the GPIO and library */
HCMotor::HCMotor(void)
{
	/* Nothing to do ! */
}

/* Initialise the interrupt timer to generate the TDM/Clock signals in the 
   background. Set the timer an interrupt to trigger every 100uS. */
void HCMotor::Init(void)
{
	HCMotor::_MotorTimer2Init(T2_CLK_DIV_32, 49);
}

/* Reconfigures the hardware timer 2 to trigger an interrupt */
void HCMotor::_MotorTimer2Init(byte prescaler, byte compare)
{
	/* Turn off interrupts whilst we setup the timer */
	cli();
	/* Set timer mode to clear timer on compare match (mode 2)*/
	TCCR2A = (1<<WGM21);

	/* Set the prescaler */
	TCCR2B = prescaler;

	/* Clear timer 2 counter */
	TCNT2 = 0;

	/* Set the compare match register */
	OCR2A = compare;

	/* Enable timer 2 interrupt on compare match */
	TIMSK2 = (1<<OCIE2A);

  	/* Turn interrupts back on */
  	sei();

}


/* Attaches a PWM output for a standard DC motor where:
MotorNum is a value used to reference which motor we wish to attach. Valid values are between 0 and MAXMOTORS.
MotorType is the type of motor being attached. Currently only one valid value (DCMOTOR)
Pin is the digital pin to attach it to (this can be any digital pin, not just the hardware PWM pins)
*/
void HCMotor::attach(byte MotorNum, byte MotorType, byte Pin)
{
	if(MotorNum < MAXMOTORS)
	{
		/* Store the port number and mask for the required digital pin */
		Motor[MotorNum].PortA = portOutputRegister(digitalPinToPort(Pin));
		Motor[MotorNum].ActivePort = Motor[MotorNum].PortA;
		
		Motor[MotorNum].MaskA = digitalPinToBitMask(Pin);
		Motor[MotorNum].ActiveMask = Motor[MotorNum].MaskA;

		/* Set all values used for generating the pulse to zero */
		Motor[MotorNum].Counter = 0;
		Motor[MotorNum].PulseHigh = 0;
		Motor[MotorNum].PulseLength = 0;
		
		/* If motor is already attached to pins then disable them before 
		   attaching to new pins */
		if (Motor[MotorNum].Attached)
		{
			pinMode(Motor[MotorNum].PinA, INPUT); 
		}else
		{
			Motor[MotorNum].Attached = true;
		}
		
		/* Save the motor type and attached pin */
		Motor[MotorNum].MotorType = MotorType;
		Motor[MotorNum].PinA = Pin;
		
		/* Set the digital pin to an output. */
		pinMode(Pin, OUTPUT);
	}
}

/* Attaches a PWM output for a standard DC motor with H-bridge driver or a stepper motor where:
MotorNum is a value used to reference which motor we wish to attach. Valid values are between 0 and MAXMOTORS.
MotorType is the type of motor being attached. Valid values are DCMOTOR_H_BRIDGE or STEPPER
PinA  PinB are the digital pins to attach it to. For a stepper motor PinA is the clock pin and PinB is the direction pin.
*/
void HCMotor::attach(byte MotorNum, byte MotorType, byte PinA, byte PinB)
{
	if(MotorNum < MAXMOTORS)
	{
		/* Store the port number and mask for the required digital pins */
		Motor[MotorNum].PortA = portOutputRegister(digitalPinToPort(PinA));
		Motor[MotorNum].PortB = portOutputRegister(digitalPinToPort(PinB));
		
		Motor[MotorNum].MaskA = digitalPinToBitMask(PinA);
		Motor[MotorNum].MaskB = digitalPinToBitMask(PinB);
		
		/* Set an active direction */
		if(Motor[MotorNum].Direction == FORWARD || Motor[MotorNum].MotorType == STEPPER)
		{
			Motor[MotorNum].ActivePort = Motor[MotorNum].PortA;
			Motor[MotorNum].ActiveMask = Motor[MotorNum].MaskA;
		}else
		{
			Motor[MotorNum].ActivePort = Motor[MotorNum].PortB;
			Motor[MotorNum].ActiveMask = Motor[MotorNum].MaskB;
		}

		/* Set all values used for generating the pulse to zero */
		Motor[MotorNum].Counter = 0;
		Motor[MotorNum].PulseHigh = 0;
		Motor[MotorNum].PulseLength = 0;
		
		/* If motor is already attached to pins then disable them before 
		   attaching to new pins */		
		if (Motor[MotorNum].Attached)
		{
			pinMode(Motor[MotorNum].PinA, INPUT);
			pinMode(Motor[MotorNum].PinB, INPUT);
		}else
		{
			Motor[MotorNum].Attached = true;
		}
		
		/* Save the motor type and attached pins */
		Motor[MotorNum].MotorType = MotorType;
		Motor[MotorNum].PinA = PinA;
		Motor[MotorNum].PinB = PinB;
		
		/* Set the digital pins to an output. */
		pinMode(PinA, OUTPUT);
		pinMode(PinB, OUTPUT);
	}
}


/* Removes an attached motor from the list and sets its pin(s) to inputs where:
   MotorNum is a value used to reference which motor we wish to detach. */
void HCMotor::detach(byte MotorNum)
{
	if (Motor[MotorNum].Attached)
	{
		Motor[MotorNum].Counter = 0;
		Motor[MotorNum].PulseHigh = 0;
		Motor[MotorNum].PulseLength = 0;
		
		pinMode(Motor[MotorNum].PinA, INPUT);
		if(Motor[MotorNum].MotorType == DCMOTOR_H_BRIDGE || Motor[MotorNum].MotorType == STEPPER)
			pinMode(Motor[MotorNum].PinB, INPUT);
		
		Motor[MotorNum].Attached = false;
	}
}
	


/* Sets the on part of the duty cycle for DC motors where:
   MotorNum is a value used to reference which motor we wish to alter.
   Time is the amount of the duty cycle the motor will be on for in 100uS increments. */
void HCMotor::OnTime(byte MotorNum, unsigned int Time)
{
	if (Motor[MotorNum].Attached && Motor[MotorNum].MotorType != STEPPER)
	{
		if(Time <= Motor[MotorNum].PulseLength)
		{
			Motor[MotorNum].PulseHigh = Time;	
		}else
		{
			Motor[MotorNum].PulseHigh = Motor[MotorNum].PulseLength;	
		}
	}
}

/* For DC motors sets the duty cycle length, for stepper motors the clock speed, where:
   MotorNum is a value used to reference which motor we wish to alter.
   Time is the duty cycle time in uS. For stepper motors this sets the clock speed. */
void HCMotor::DutyCycle(byte MotorNum, unsigned int Time)
{
	if (Motor[MotorNum].Attached)
	{
		Motor[MotorNum].PulseLength = Time;
		if(Motor[MotorNum].PulseHigh > Motor[MotorNum].PulseLength)
			Motor[MotorNum].PulseHigh = Motor[MotorNum].PulseLength;
		
		if(Motor[MotorNum].MotorType == STEPPER)
			Motor[MotorNum].PulseHigh = Motor[MotorNum].PulseLength >> 1;

	}
}

/* Sets the direction of the motor for H-Bridge and stepper motors where:
	MotorNum is a value used to reference which motor we wish to alter.
	Direction is the direction of the motor. Valid values are FORWARD & REVERSE
	
	For H-Bridge motors this will switch the pin the PWM is output to and the sets the opposite pin low.
	For stepper motors the will set the direction pin (PinB) high or low. */
	
void HCMotor::Direction(byte MotorNum, boolean Direction)
{
	if(Motor[MotorNum].MotorType == DCMOTOR_H_BRIDGE)
	{
		/* If H-Bridge motor then swat the PWM pin */
		if(Direction == FORWARD)
		{
			Motor[MotorNum].ActivePort = Motor[MotorNum].PortA;
			Motor[MotorNum].ActiveMask = Motor[MotorNum].MaskA;
			digitalWrite(Motor[MotorNum].PinB, LOW);
		}else
		{
			Motor[MotorNum].ActivePort = Motor[MotorNum].PortB;
			Motor[MotorNum].ActiveMask = Motor[MotorNum].MaskB;
			digitalWrite(Motor[MotorNum].PinA, LOW);
		}				
	/* If Stepper motor then change the direction pin */
	}else if(Motor[MotorNum].MotorType == STEPPER)
	{
		digitalWrite(Motor[MotorNum].PinB, Direction);
	}
	
	Motor[MotorNum].Direction = Direction;
}	

/* Sets the number of steps (pulses) to step a stepper motor where:
   MotorNum is a value used to reference which motor we wish to alter.
   Steps is the number of steps to step the motor.
   
   Note that for DC motors this function does nothing. */
void HCMotor::Steps(byte MotorNum, unsigned int Steps)
{
	Motor[MotorNum].Steps = Steps;
}


/* Interrupt service routine for Timer 2 compare match */
ISR(TIMER2_COMPA_vect)
{
	byte Index;
	
	/* Step through each motor */
	for (Index = 0; Index < MAXMOTORS; Index++)
	{
		/* Check if the motor is attached, duty cycle is not zero, and if it is
 		   a stepper motor number of steps is not zero */
		if(Motor[Index].PulseLength && 
		   Motor[Index].Attached && 
		   (Motor[Index].MotorType != STEPPER || Motor[Index].Steps))
		{
			/* Tick up the counter time */
			Motor[Index].Counter++;

			/* If the counter time is still in the ON time then set the PWM/Clock pin high */
			if (Motor[Index].Counter <= Motor[Index].PulseHigh)
			{
				*Motor[Index].ActivePort |= Motor[Index].ActiveMask;
			/* Else set the pin low */
			}else
			{
				*Motor[Index].ActivePort &= ~Motor[Index].ActiveMask;
			}
  
			/* If the counter time has reached the end of the duty cycle then reset it */
			if (Motor[Index].Counter >= Motor[Index].PulseLength)
			{
				Motor[Index].Counter = 0;
			/* If motor is a stepper motor then reduce the step counter by one */
			if (Motor[Index].MotorType == STEPPER && Motor[Index].Steps && Motor[Index].Steps != CONTINUOUS)
				Motor[Index].Steps--;
			}
		}
	}	
}
