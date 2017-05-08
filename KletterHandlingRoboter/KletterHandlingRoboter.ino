/*
 Name:		KletterHandlingRoboter.ino
 Created:	14.01.2017 08:37:39
 Author:	Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

// libary
/////////////////////////////////////////////////////////////////////

#include <IRremote.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "TrackSensor.h"
#include "Timer.h"


// variables
/////////////////////////////////////////////////////////////////////

// const int
const int
	sn_SensorFrontPin		= 3,				// pin for front sensor
	sn_SensorCenterPin		= 4,				// pin for center sensor
	sn_SensorBackPin		= 5,				// pin for back sensor
	sn_GripperServoPin		= 9,				// pin for gripper servo
	sn_ArmServoPin			= 10,				// pin for arm servo
	sn_IRPin				= 7,				// pin for IR remote
	sn_BaudRate				= 9600,				// baud rate for serial communication
	sn_ShieldAdress			= 0x60,				// motor shield adress
	sn_StepperResolution	= 200				// steps / u ( 360° / 1.8° per step)
;

// int
int
	sn_Sequencer			= 0					// main sequence						
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

IRrecv IRRx(sn_IRPin);
decode_results Results;

// functions
/////////////////////////////////////////////////////////////////////

// main sequencer
void MainSequence() {

	// variables
	/////////////////////////////////////////////////////////////////////

	// bool


	// unsignd long


	// objects
	/////////////////////////////////////////////////////////////////////

	
	// start timer
	/////////////////////////////////////////////////////////////////////

	
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

	// variables
	/////////////////////////////////////////////////////////////////////

	// arm servo
	/////////////////////////////////////////////////////////////////////

	// gripper servo
	/////////////////////////////////////////////////////////////////////

	// stepper
	/////////////////////////////////////////////////////////////////////
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

	IRRx.enableIRIn();							// start the IR receiver

	pinMode(51, OUTPUT);
	pinMode(50, INPUT);
}

// loop
void loop() {
	

	// call functions
	
	static TOF ton_Timer;

	void MainSequence();						// main controll sequence
	void Outputs();								// outputs
	

	
	bool pin = digitalRead(50);
	bool out = false;
	unsigned long pt = 5000;
	unsigned long et;

	
	ton_Timer.in(pin);
	ton_Timer.et(et);
	ton_Timer.pt(pt);
	ton_Timer.q(out);
	digitalWrite(51,out);
					
	/*
	if (IRRx.decode(&Results)) {
		Serial.println(Results.value, HEX);
		IRRx.resume();
	}
	delay(100);

	/*
	
	Serial.print("SensorFront = ");
	Serial.println(SensorFront.result());
	
	
	if (SensorFront.result()){
		Stepper->onestep(FORWARD, DOUBLE);
	}

	ArmServo.writeMicroseconds(910);
	*/
}