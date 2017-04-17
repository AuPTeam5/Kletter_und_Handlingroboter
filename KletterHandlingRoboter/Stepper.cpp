/*
Name:		Stepper.cpp
Created:	17.04.2017 15:32:00
Author:		Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#include "Stepper.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

uint16_t _steps;
uint8_t _n;
uint8_t _addr;

Adafruit_MotorShield AFMS(_addr);
Adafruit_StepperMotor *StepperDrive();

Stepper::Stepper(uint16_t steps, uint8_t n, uint8_t addr = 0x60) {
	_addr = addr;
	_steps = steps;
	_n = n;
}

void Stepper::begin(){
	AFMS.getStepper(_steps, _n);
	AFMS.begin();
}

void Stepper::move(int speed, int stepps, uint8_t direction, uint8_t steptype) {
	_speed = speed;
	_stepps = stepps;
	_direction = direction;
	_steptype = steptype;
	StepperDrive->setSpeed(_speed);
	StepperDrive->step(_stepps, _direction, _steptype);	
}