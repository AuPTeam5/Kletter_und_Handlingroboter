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
const int BaudRate = 9600;				// baud rate for serial communication

// int
int	Sequencer = 0;						// main sequence						

// enum
enum State {							// enumeration for sequencer
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


// functions
/////////////////////////////////////////////////////////////////////

// main sequencer
void MainSequence() {

	// variables
	/////////////////////////////////////////////////////////////////////

	// static bool
	static bool
		firstCycle = true,
		SequencerTimerIN = false,
		SequencerTimerOUT = false;

	// static long
		static long
		SequencerTimerPt = 0,
		SequencerTimerEt = 0;
	
	// const int
	const int
		SensorFrontPin = 3,				// pin for front sensor
		SensorCenterPin = 4,			// pin for center sensor
		SensorBackPin = 5,				// pin for back sensor
		IRPin = 7						// pin for IR remote
		;

	// const unsigned long
	const unsigned long 
		IRRxStart = 16761405,			// start signal (  PLAY/PAUSE button )
		IRRxReset = 16736925,			// start signal ( CH button )
		GripperTime = 2000				// timer for gripper movement
		;

	// objects
	/////////////////////////////////////////////////////////////////////

	static TrackSensor SensorFront(SensorFrontPin);
	static TrackSensor SensorCenter(SensorCenterPin);
	static TrackSensor SensorBack(SensorBackPin);
	static IRrecv IRRx(IRPin);
	static decode_results Results;
	static TON SequencerTimer;

	// init
	/////////////////////////////////////////////////////////////////////
	
	if (firstCycle)
	{
		IRRx.enableIRIn();				// start the IR receiver
	}

	// timer
	/////////////////////////////////////////////////////////////////////
	SequencerTimer.in(SequencerTimerIN);
	SequencerTimer.pt(SequencerTimerPt);
	SequencerTimerEt = SequencerTimer.et();
	SequencerTimerOUT = SequencerTimer.q();

	// sequence
	/////////////////////////////////////////////////////////////////////

	Serial.println(Sequencer, DEC);

	switch (Sequencer){

	// init
	case start:
		if (IRRx.decode(&Results))
		{
			if (IRRxStart == Results.value)
			{
				Sequencer = move_to_pipe_bottom;
			}
			IRRx.resume();
		}
		break;

	// move to pipe bottom position
	case move_to_pipe_bottom:

		break;

	// close gripper bottom pos
	case grip_pipe_bottom:

		break;
		
	// move pipe out bottom
	case drag_pipe_out_bottom:
		SequencerTimerPt = GripperTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = grip_pipe_bottom;
		}
		break;

	// pepare arm for move to top
	case move_arm_in_transport_pos_to_top:

		break;

	// move to top position
	case move_to_top:

		break;

	// move arm in release position on top
	case move_arm_to_release_pos_top:

		break;

	// open gripper
	case release_pipe_top:

		break;

	// move to center
	case move_to_center:

		break;

	// wait 5s
	case wait_5s:

		break;

	// move to pipe top position
	case move_to_pipe_top:

		break;

	// close gripper on top pos
	case grip_pipe_top:

		break;

	// move pipe out top
	case drag_pipe_out_top:

		break;

	// move arm in transport pposition to bottom
	case move_arm_in_transport_pos_to_bottom:

		break;

	// move to bottom
	case move_to_bottom:

		break;

	// open gripper
	case release_pipe_bottom:

		break;

	// move to endposition
	case move_to_endpos:

		break;

	// end
	case State(end):
		if (IRRx.decode(&Results))
		{
			if (IRRxReset == Results.value)
			{
				Sequencer = start;
			}
			IRRx.resume();
		}
		break;

	// reset by programm bug
	default:
		Sequencer = start;
		break;
	}

	// reset first cycle variable
	/////////////////////////////////////////////////////////////////////

	firstCycle = false;
}

// outputs
void Outputs(){

	// variables
	/////////////////////////////////////////////////////////////////////

	// static bool
	static bool firstCycle = true;

	// const int
	const int
		GripperServoPin = 9,					// pin for gripper servo
		ArmServoPin = 10,						// pin for arm servo
		ShieldAdress = 0x60,					// motor shield adress
		StepperResolution = 200					// steps / u ( 360° / 1.8° per step)
		;


	// objects
	/////////////////////////////////////////////////////////////////////

	static Adafruit_MotorShield AFMS = Adafruit_MotorShield(ShieldAdress);
	static Adafruit_StepperMotor *Stepper = AFMS.getStepper(StepperResolution, 1);
	static Servo GripperServo;
	static Servo ArmServo;

	// init
	/////////////////////////////////////////////////////////////////////

	if (firstCycle)
	{
		AFMS.begin();								// start motor shield
		GripperServo.attach(GripperServoPin);		// attach GripperServo to pin 2
		ArmServo.attach(ArmServoPin);				// attach ArmServo to pin 3
	} 

	// arm servo
	/////////////////////////////////////////////////////////////////////

	// gripper servo
	/////////////////////////////////////////////////////////////////////

	// stepper
	/////////////////////////////////////////////////////////////////////

	// reset first cycle variable
	/////////////////////////////////////////////////////////////////////

	firstCycle = false;
}

// setup
void setup() {

	Serial.begin(BaudRate);					// start serial communication
	Sequencer = 0;							// set sequence to init
}

// loop
void loop() {
	
	// call functions
	/////////////////////////////////////////////////////////////////////
	
	MainSequence();							// main controll sequence
	Outputs();								// outputs
}