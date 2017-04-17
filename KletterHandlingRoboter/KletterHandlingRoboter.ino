/*
 Name:		KletterHandlingRoboter.ino
 Created:	14.01.2017 08:37:39
 Author:	Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#include "TrackSensor.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_StepperMotor *Stepper1 = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *Stepper2 = AFMS.getStepper(200, 2);
TrackSensor Sensor1(0);
TrackSensor Sensor2(1);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	AFMS.begin();
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.print("Sensor1 = ");
	Serial.println(Sensor1.result());
	delay(1000);
	Serial.print("Sensor2 = ");
	Serial.println(Sensor2.result());
	delay(1000);
	if (! Sensor1.result()){
		stepper1(1000, 1000, FORWARD, DOUBLE);		
	}
}


// stepper controll 1
void stepper1(int speed, int stepps, uint8_t direction, uint8_t steptype) {
	Stepper1->setSpeed(speed);
	Stepper1->step(stepps, direction, steptype);
}
// stepper controll 2
void stepper2(int speed, int stepps, uint8_t direction, uint8_t steptype) {
	Stepper2->setSpeed(speed);
	Stepper2->step(stepps, direction, steptype);
}
