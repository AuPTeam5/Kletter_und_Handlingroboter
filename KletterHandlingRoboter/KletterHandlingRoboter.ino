/*
 Name:		KletterHandlingRoboter.ino
 Created:	14.01.2017 08:37:39
 Author:	Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#include <IRremote.h>
#include <Servo.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include "TrackSensor.h"


// variables
/////////////////////////////////////////////////////////////////////

// const int
const int	
	RxPin			= 2,					// pin for IR reciver
	SensorFrontPin	= 0,					// pin for front sensor
	SensorCenterPin = 1,					// pin for center sensor
	SensorBackPin	= 2						// pin for back sensor
;

// enum
enum state {								// enumeration for sequencer
	init	= 0,
	end		= 100
};

// objects
/////////////////////////////////////////////////////////////////////

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);
Adafruit_StepperMotor *StepperDrive1 = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *StepperDrive2 = AFMS.getStepper(200, 2);
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

TrackSensor SensorFront(SensorFrontPin);
TrackSensor SensorCenter(SensorCenterPin);
TrackSensor SensorBack(SensorBackPin);

Servo GripperServo;
Servo ArmServo;

IRrecv Remote(RxPin);
decode_results IRResults;

// functions
/////////////////////////////////////////////////////////////////////

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

// main sequencer
void MainSequence(int sequencer) {

	// sequence
	switch (sequencer){

	// init
	case state(init):

		break;

	// end
	case state(end):

		break;

	// reset by programm bug
	default:
		sequencer = state(init);
		break;
	}

};

// main programm
/////////////////////////////////////////////////////////////////////

// setup
void setup() {
	Serial.begin(9600);							// start serial communication
	
	AFMS.begin();								// start motor shield
	stepper1.setMaxSpeed(200.0);				// set max speed for stepper 1	
	stepper1.setAcceleration(100.0);			// set acceleration for stepper 1
	stepper2.setMaxSpeed(200.0);				// set max speed for stepper 2
	stepper2.setAcceleration(100.0);			// set acceleration for stepper 2

	Remote.enableIRIn();						// start the receiver

	GripperServo.attach(2);

	MainSequence(0);							// set sequence to init

}

// loop
void loop() {
	
	/*
	Serial.print("SensorFront = ");
	Serial.println(SensorFront.result());
	*/
	
	if (SensorFront.result()){
		stepper1.setSpeed(200);
		stepper1.runSpeed();
	}

	ArmServo.writeMicroseconds(500);
	GripperServo.writeMicroseconds(1010);
	delay(2000);
	ArmServo.writeMicroseconds(2500);
	GripperServo.writeMicroseconds(1100);
	delay(2000);
}