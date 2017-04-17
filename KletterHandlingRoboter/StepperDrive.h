/*
Name:		StepperDrive.h
Created:	08.04.2017 15:44:00
Author:		Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#ifndef _StepperDrive_h
#define _StepperDrive_h

#include "Arduino.h"
#include <Wire.h>
#include "StepperDrive.h"
#include "Adafruit_MS_PWMServoDriver.h"

//#define MOTORDEBUG

#define MICROSTEPS 16         // 8 or 16

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

class StepperDrive {
public:
	StepperDrive(void);
	friend class MotorShield;

	void step(uint16_t steps, uint8_t dir, uint8_t style = SINGLE);
	void setSpeed(uint16_t);
	uint8_t onestep(uint8_t dir, uint8_t style);
	void release(void);
	uint32_t usperstep;

private:
	uint8_t PWMApin, AIN1pin, AIN2pin;
	uint8_t PWMBpin, BIN1pin, BIN2pin;
	uint16_t revsteps;
	uint8_t currentstep;
	MotorShield *MC;
	uint8_t steppernum;
};

class MotorShield
{
public:
	MotorShield(uint8_t addr = 0x60);
	void begin(uint16_t freq = 1600);

	void setPWM(uint8_t pin, uint16_t val);
	void setPin(uint8_t pin, boolean val);
	StepperDrive *getStepper(uint16_t steps, uint8_t n);
private:
	uint8_t _addr;
	uint16_t _freq;
	StepperDrive steppers[2];
	Adafruit_MS_PWMServoDriver _pwm;
};

#endif


