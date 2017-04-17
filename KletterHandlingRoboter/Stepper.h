/*
Name:		Stepper.h
Created:	17.04.2017 15:32:00
Author:		Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#ifndef _STEPPER_h
#define _STEPPER_h

#include "arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

class Stepper {

public:
	Stepper(uint16_t steps, uint8_t n, uint8_t addr = 0x60);
	void move(int speed, int stepps, uint8_t direction, uint8_t steptype);
	void begin();

private:
	uint16_t _steps;
	uint8_t _n;
	uint8_t _addr;
	int _speed;
	int _stepps;
	uint8_t _direction;
	uint8_t _steptype;
	Adafruit_MotorShield AFMS;
	Adafruit_StepperMotor *StepperDrive;
};

#endif
