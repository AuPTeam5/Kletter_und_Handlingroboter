/*
Name:		StepperDrve.cpp
Created:	17.04.2017 15:44:00
Author:		Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>
#include "StepperDrive.h"
#include "Adafruit_MS_PWMServoDriver.h"

#if defined(ARDUINO_SAM_DUE)
#define WIRE Wire1
#else
#define WIRE Wire
#endif

#if (MICROSTEPS == 8)
uint8_t microstepcurve[] = { 0, 50, 98, 142, 180, 212, 236, 250, 255 };
#elif (MICROSTEPS == 16)
uint8_t microstepcurve[] = { 0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255 };
#endif

MotorShield::MotorShield(uint8_t addr) {
	_addr = addr;
	_pwm = Adafruit_MS_PWMServoDriver(_addr);
}

void MotorShield::begin(uint16_t freq) {
	// init PWM w/_freq
	WIRE.begin();
	_pwm.begin();
	_freq = freq;
	_pwm.setPWMFreq(_freq);  // This is the maximum PWM frequency
	for (uint8_t i = 0; i<16; i++)
		_pwm.setPWM(i, 0, 0);
}

void MotorShield::setPWM(uint8_t pin, uint16_t value) {
	if (value > 4095) {
		_pwm.setPWM(pin, 4096, 0);
	}
	else
		_pwm.setPWM(pin, 0, value);
}
void MotorShield::setPin(uint8_t pin, boolean value) {
	if (value == LOW)
		_pwm.setPWM(pin, 0, 0);
	else
		_pwm.setPWM(pin, 4096, 0);
}

StepperDrive *MotorShield::getStepper(uint16_t steps, uint8_t num) {
	if (num > 2) return NULL;

	num--;

	if (steppers[num].steppernum == 0) {
		// not init'd yet!
		steppers[num].steppernum = num;
		steppers[num].revsteps = steps;
		steppers[num].MC = this;
		uint8_t pwma, pwmb, ain1, ain2, bin1, bin2;
		if (num == 0) {
			pwma = 8; ain2 = 9; ain1 = 10;
			pwmb = 13; bin2 = 12; bin1 = 11;
		}
		else if (num == 1) {
			pwma = 2; ain2 = 3; ain1 = 4;
			pwmb = 7; bin2 = 6; bin1 = 5;
		}
		steppers[num].PWMApin = pwma;
		steppers[num].PWMBpin = pwmb;
		steppers[num].AIN1pin = ain1;
		steppers[num].AIN2pin = ain2;
		steppers[num].BIN1pin = bin1;
		steppers[num].BIN2pin = bin2;
	}
	return &steppers[num];
}


/******************************************
MOTORS
******************************************/

/******************************************
STEPPERS
******************************************/

StepperDrive::StepperDrive(void) {
	revsteps = steppernum = currentstep = 0;
}

void StepperDrive::setSpeed(uint16_t rpm) {
	//Serial.println("steps per rev: "); Serial.println(revsteps);
	//Serial.println("RPM: "); Serial.println(rpm);

	usperstep = 60000000 / ((uint32_t)revsteps * (uint32_t)rpm);
}

void StepperDrive::release(void) {
	MC->setPin(AIN1pin, LOW);
	MC->setPin(AIN2pin, LOW);
	MC->setPin(BIN1pin, LOW);
	MC->setPin(BIN2pin, LOW);
	MC->setPWM(PWMApin, 0);
	MC->setPWM(PWMBpin, 0);
}

void StepperDrive::step(uint16_t steps, uint8_t dir, uint8_t style) {
	uint32_t uspers = usperstep;
	uint8_t ret = 0;

	if (style == INTERLEAVE) {
		uspers /= 2;
	}
	else if (style == MICROSTEP) {
		uspers /= MICROSTEPS;
		steps *= MICROSTEPS;
#ifdef MOTORDEBUG
		Serial.print("steps = "); Serial.println(steps, DEC);
#endif
	}

	while (steps--) {
		//Serial.println("step!"); Serial.println(uspers);
		ret = onestep(dir, style);
		delayMicroseconds(uspers);
		yield(); // required for ESP8266
	}
}

uint8_t StepperDrive::onestep(uint8_t dir, uint8_t style) {
	uint8_t a, b, c, d;
	uint8_t ocrb, ocra;

	ocra = ocrb = 255;


	// next determine what sort of stepping procedure we're up to
	if (style == SINGLE) {
		if ((currentstep / (MICROSTEPS / 2)) % 2) { // we're at an odd step, weird
			if (dir == FORWARD) {
				currentstep += MICROSTEPS / 2;
			}
			else {
				currentstep -= MICROSTEPS / 2;
			}
		}
		else {           // go to the next even step
			if (dir == FORWARD) {
				currentstep += MICROSTEPS;
			}
			else {
				currentstep -= MICROSTEPS;
			}
		}
	}
	else if (style == DOUBLE) {
		if (!(currentstep / (MICROSTEPS / 2) % 2)) { // we're at an even step, weird
			if (dir == FORWARD) {
				currentstep += MICROSTEPS / 2;
			}
			else {
				currentstep -= MICROSTEPS / 2;
			}
		}
		else {           // go to the next odd step
			if (dir == FORWARD) {
				currentstep += MICROSTEPS;
			}
			else {
				currentstep -= MICROSTEPS;
			}
		}
	}
	else if (style == INTERLEAVE) {
		if (dir == FORWARD) {
			currentstep += MICROSTEPS / 2;
		}
		else {
			currentstep -= MICROSTEPS / 2;
		}
	}

	if (style == MICROSTEP) {
		if (dir == FORWARD) {
			currentstep++;
		}
		else {
			// BACKWARDS
			currentstep--;
		}

		currentstep += MICROSTEPS * 4;
		currentstep %= MICROSTEPS * 4;

		ocra = ocrb = 0;
		if ((currentstep >= 0) && (currentstep < MICROSTEPS)) {
			ocra = microstepcurve[MICROSTEPS - currentstep];
			ocrb = microstepcurve[currentstep];
		}
		else if ((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS * 2)) {
			ocra = microstepcurve[currentstep - MICROSTEPS];
			ocrb = microstepcurve[MICROSTEPS * 2 - currentstep];
		}
		else if ((currentstep >= MICROSTEPS * 2) && (currentstep < MICROSTEPS * 3)) {
			ocra = microstepcurve[MICROSTEPS * 3 - currentstep];
			ocrb = microstepcurve[currentstep - MICROSTEPS * 2];
		}
		else if ((currentstep >= MICROSTEPS * 3) && (currentstep < MICROSTEPS * 4)) {
			ocra = microstepcurve[currentstep - MICROSTEPS * 3];
			ocrb = microstepcurve[MICROSTEPS * 4 - currentstep];
		}
	}

	currentstep += MICROSTEPS * 4;
	currentstep %= MICROSTEPS * 4;

	MC->setPWM(PWMApin, ocra * 16);
	MC->setPWM(PWMBpin, ocrb * 16);


	// release all
	uint8_t latch_state = 0; // all motor pins to 0

							 //Serial.println(step, DEC);
	if (style == MICROSTEP) {
		if ((currentstep >= 0) && (currentstep < MICROSTEPS))
			latch_state |= 0x03;
		if ((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS * 2))
			latch_state |= 0x06;
		if ((currentstep >= MICROSTEPS * 2) && (currentstep < MICROSTEPS * 3))
			latch_state |= 0x0C;
		if ((currentstep >= MICROSTEPS * 3) && (currentstep < MICROSTEPS * 4))
			latch_state |= 0x09;
	}
	else {
		switch (currentstep / (MICROSTEPS / 2)) {
		case 0:
			latch_state |= 0x1; // energize coil 1 only
			break;
		case 1:
			latch_state |= 0x3; // energize coil 1+2
			break;
		case 2:
			latch_state |= 0x2; // energize coil 2 only
			break;
		case 3:
			latch_state |= 0x6; // energize coil 2+3
			break;
		case 4:
			latch_state |= 0x4; // energize coil 3 only
			break;
		case 5:
			latch_state |= 0xC; // energize coil 3+4
			break;
		case 6:
			latch_state |= 0x8; // energize coil 4 only
			break;
		case 7:
			latch_state |= 0x9; // energize coil 1+4
			break;
		}
	}

	if (latch_state & 0x1) {
		// Serial.println(AIN2pin);
		MC->setPin(AIN2pin, HIGH);
	}
	else {
		MC->setPin(AIN2pin, LOW);
	}
	if (latch_state & 0x2) {
		MC->setPin(BIN1pin, HIGH);
		// Serial.println(BIN1pin);
	}
	else {
		MC->setPin(BIN1pin, LOW);
	}
	if (latch_state & 0x4) {
		MC->setPin(AIN1pin, HIGH);
		// Serial.println(AIN1pin);
	}
	else {
		MC->setPin(AIN1pin, LOW);
	}
	if (latch_state & 0x8) {
		MC->setPin(BIN2pin, HIGH);
		// Serial.println(BIN2pin);
	}
	else {
		MC->setPin(BIN2pin, LOW);
	}

	return currentstep;
}