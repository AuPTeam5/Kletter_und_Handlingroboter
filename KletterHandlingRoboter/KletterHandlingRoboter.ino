/*
 Name:		KletterHandlingRoboter.ino
 Created:	14.01.2017 08:37:39
 Author:	Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#include <Servo.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include "TrackSensor.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_StepperMotor *StepperDrive1 = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *StepperDrive2 = AFMS.getStepper(200, 2);

TrackSensor Sensor1(0);
TrackSensor Sensor2(1);

Servo GripperServo;

// wrappers for the first motor!
void forwardstep1() {
	StepperDrive1->onestep(FORWARD, SINGLE);
}
void backwardstep1() {
	StepperDrive1->onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {
	StepperDrive2->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {
	StepperDrive2->onestep(BACKWARD, DOUBLE);
}
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	
	AFMS.begin();
	stepper1.setMaxSpeed(200.0);
	stepper1.setAcceleration(100.0);
	stepper2.setMaxSpeed(200.0);
	stepper2.setAcceleration(100.0);


	GripperServo.attach(2);
}

// the loop function runs over and over again until power down or reset
void loop() {
	
	/*
	Serial.print("Sensor1 = ");
	Serial.println(Sensor1.result());
	
	Serial.print("Sensor2 = ");
	Serial.println(Sensor2.result());
	*/
	
	if (Sensor1.result()){
		stepper1.setSpeed(200);
		stepper1.runSpeed();
	}

	
	//test
	

	GripperServo.writeMicroseconds(500);
	delay(2000);
	GripperServo.writeMicroseconds(2500);
	delay(2000);
}