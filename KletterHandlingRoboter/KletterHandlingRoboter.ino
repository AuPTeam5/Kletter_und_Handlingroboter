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
	RxPin				= 2,				// pin for IR reciver
	SensorFrontPin		= 0,				// pin for front sensor
	SensorCenterPin		= 1,				// pin for center sensor
	SensorBackPin		= 2,				// pin for back sensor
	ShieldAdress		= 0X60				// motor shield adress
;

// const long
const double
	Stepper1Resolution	= 1.8,				// degree per step
	Stepper1MaxSpeed	= 200.0,			// max speed for stepper 1
	Stepper1Accl		= 100.0				// acceleration for stepper 1
;

// enum
enum state {								// enumeration for sequencer
	init	= 0,
	end		= 100
};

// objects
/////////////////////////////////////////////////////////////////////

Adafruit_MotorShield AFMS = Adafruit_MotorShield(ShieldAdress);
Adafruit_StepperMotor *StepperDrive1 = AFMS.getStepper((360 / Stepper1Resolution), 1);
AccelStepper stepper1(forwardstep1, backwardstep1);

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
	stepper1.setMaxSpeed(Stepper1MaxSpeed);		// set max speed for stepper 1	
	stepper1.setAcceleration(Stepper1Accl);		// set acceleration for stepper 1

	Remote.enableIRIn();						// start the receiver

	GripperServo.attach(2);						// attach GripperServo to pin 2
	ArmServo.attach(3);							// attach ArmServo to pin 3

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