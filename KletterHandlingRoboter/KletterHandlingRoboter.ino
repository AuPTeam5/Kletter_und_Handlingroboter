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
	start,
	move_to_pipe_bottom,
	grip_pipe_bottom,
	drag_pipe_out_bottom,
	move_arm_in_transport_pos_to_top,
	move_to_top,
	move_arm_to_release_pos_top,
	release_pipe_top,
	move_to_center,
	wait_5s,
	move_to_pipe_top,
	grip_pipe_top,
	drag_pipe_out_top,
	move_arm_in_transport_pos_to_bottom,
	move_to_bottom,
	release_pipe_bottom,
	move_to_endpos,
	end
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
	case state(start):

		break;

	// move to pipe bottom position
	case state(move_to_pipe_bottom):

		break;

	// close gripper bottom pos
	case state(grip_pipe_bottom):

		break;
		
	// move pipe out bottom
	case state(drag_pipe_out_bottom):

		break;

	// pepare arm for move to top
	case state(move_arm_in_transport_pos_to_top):

		break;

	// move to top position
	case state(move_to_top):

		break;

	// move arm in release position on top
	case state(move_arm_to_release_pos_top):

		break;

	// open gripper
	case state(release_pipe_top):

		break;

	// move to center
	case state(move_to_center):

		break;

	// wait 5s
	case state(wait_5s):

		break;

	// move to pipe top position
	case state(move_to_pipe_top):

		break;

	// close gripper on top pos
	case state(grip_pipe_top):

		break;

	// move pipe out top
	case state(drag_pipe_out_top):

		break;

	// move arm in transport pposition to bottom
	case state(move_arm_in_transport_pos_to_bottom):

		break;

	// move to bottom
	case state(move_to_bottom):

		break;

	// open gripper
	case state(release_pipe_bottom):

		break;

	// move to endposition
	case state(move_to_endpos):

		break;

	// end
	case state(end):

		break;

	// reset by programm bug
	default:
		sequencer = state(start);
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
	
	// call functions
	void MainSequence();

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