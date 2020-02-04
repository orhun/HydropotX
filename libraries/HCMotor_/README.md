# HCMotor
An Arduino library for controlling DC and stepper motors

![alt tag](http://hobbycomponents.com/images/forum/HCMotor/HCMotor_DC_Motor_With_H_Bridge.JPG)

This Arduino library (current only supports ATMega328p based Arduinos) will allow you to control one or more DC or stepper motors from you Arduino. The library makes use of the Arduinos hardware interrupt timer 2 to drive the motors in the background leaving your sketch totally free for you main program. It also allows you to mix motor types and connect them to any available digital pins (you are not limited to just PWM pins). You can simply add motors by using the libraries 'Attach' function and remove them using the 'Detach' command. You can also mix supported motor types. Current supported motor types are as follows:

DCMOTOR - A standard DC type motor driven via a transistor or driver module.

DCMOTOR_H_BRIDGE - A standard DC motor connected via a H-Bridge driver module allowing for forward and reverse directions.

STEPPER - A stepper motor connected via a standard stepper motor driver (Step/CLK & Direction)




####**For suitable motor drivers see the following:**

For driving DC motor in one direction
http://hobbycomponents.com/transistors/507-12n10l-fet-transistor-rfp12n10

For driving DC motor in both forward and reverse
http://hobbycomponents.com/modules/264-l9110s-dc-stepper-motor-driver-h-bridge
http://hobbycomponents.com/modules/54-l298n-stepper-motor-driver-controller-board

For driving a stepper motor
http://hobbycomponents.com/other/547-stepper-motor-driver-a4988
http://hobbycomponents.com/modules/249-tb65603a-single-axis-stepper-motor-driver-board



To use this library  you will need to download and unzip this library to the Arduino development environments library area.

On Windows: 
My Documents\Arduino\libraries\

On Mac: 
Documents/Arduino/libraries/
or similarly for Linux.

More information on how to use this library can be found in the software section of our support forum:
http://forum.hobbycomponents.com/viewtopic.php?f=58&t=1870&p=4468#p4468
