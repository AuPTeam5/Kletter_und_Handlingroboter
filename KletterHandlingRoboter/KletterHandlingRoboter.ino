/*
 Name:		KletterHandlingRoboter.ino
 Created:	14.01.2017 08:37:39
 Author:	Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

// libary
/////////////////////////////////////////////////////////////////////

#include <Servo.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "TrackSensor.h"
#include "iecTimer.h"


// variables
/////////////////////////////////////////////////////////////////////

// const int
const int
	sn_SensorFrontPin = 0,				// pin for front sensor
	sn_SensorCenterPin = 1,				// pin for center sensor
	sn_SensorBackPin = 2,				// pin for back sensor
	sn_GripperServoPin = 2,				// pin for gripper servo
	sn_ArmServoPin = 3,					// pin for arm servo
	sn_BaudRate = 9600,					// baud rate for serial communication
	sn_ShieldAdress			= 0x60,		// motor shield adress
	sn_StepperResolution	= 200		// steps / u ( 360� / 1.8� per step)
;

// int
int
	sn_Sequencer			= 0			// main sequence						
;

// enum
enum e_State {							// enumeration for sequencer
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

Adafruit_MotorShield AFMS = Adafruit_MotorShield(sn_ShieldAdress);
Adafruit_StepperMotor *Stepper = AFMS.getStepper(sn_StepperResolution, 1);

TrackSensor SensorFront(sn_SensorFrontPin);
TrackSensor SensorCenter(sn_SensorCenterPin);
TrackSensor SensorBack(sn_SensorBackPin);

Servo GripperServo;
Servo ArmServo;

// functions
/////////////////////////////////////////////////////////////////////

// main sequencer
void MainSequence() {

	// variables
	/////////////////////////////////////////////////////////////////////

	// bool
	bool
		is_inTimer				= false,	// start input
		is_outTimer				= false;	// timer out put

	// unsignd long
	unsigned long 
		ul_PT					= 500,		// programmed time in [ms]
		ul_ET					= 0;		// established time im [ms]


	// objects
	/////////////////////////////////////////////////////////////////////

	Ton ton_Sequencer(&is_inTimer, &is_outTimer, &ul_PT, &ul_ET);

	// start timer
	/////////////////////////////////////////////////////////////////////

	ton_Sequencer.run();					// activate timer

	// sequence
	/////////////////////////////////////////////////////////////////////

	switch (sn_Sequencer){

	// init
	case e_State(start):

		break;

	// move to pipe bottom position
	case e_State(move_to_pipe_bottom):

		break;

	// close gripper bottom pos
	case e_State(grip_pipe_bottom):

		break;
		
	// move pipe out bottom
	case e_State(drag_pipe_out_bottom):

		break;

	// pepare arm for move to top
	case e_State(move_arm_in_transport_pos_to_top):

		break;

	// move to top position
	case e_State(move_to_top):

		break;

	// move arm in release position on top
	case e_State(move_arm_to_release_pos_top):

		break;

	// open gripper
	case e_State(release_pipe_top):

		break;

	// move to center
	case e_State(move_to_center):

		break;

	// wait 5s
	case e_State(wait_5s):

		break;

	// move to pipe top position
	case e_State(move_to_pipe_top):

		break;

	// close gripper on top pos
	case e_State(grip_pipe_top):

		break;

	// move pipe out top
	case e_State(drag_pipe_out_top):

		break;

	// move arm in transport pposition to bottom
	case e_State(move_arm_in_transport_pos_to_bottom):

		break;

	// move to bottom
	case e_State(move_to_bottom):

		break;

	// open gripper
	case e_State(release_pipe_bottom):

		break;

	// move to endposition
	case e_State(move_to_endpos):

		break;

	// end
	case e_State(end):

		break;

	// reset by programm bug
	default:
		sn_Sequencer = e_State(start);
		break;
	}

};

// outputs
void Outputs(){
};

// main programm
/////////////////////////////////////////////////////////////////////

// setup
void setup() {
	Serial.begin(sn_BaudRate);					// start serial communication
	
	AFMS.begin();								// start motor shield

	GripperServo.attach(sn_GripperServoPin);	// attach GripperServo to pin 2
	ArmServo.attach(sn_ArmServoPin);			// attach ArmServo to pin 3

	sn_Sequencer = 0;							// set sequence to init
}

// loop
void loop() {
	
	// call functions
	void MainSequence();						// main controll sequence
	void Outputs();								// outputs

	/*
	Serial.print("SensorFront = ");
	Serial.println(SensorFront.result());
	
	
	if (SensorFront.result()){
		Stepper->onestep(FORWARD, DOUBLE);
	}

	ArmServo.writeMicroseconds(910);
	*/
}