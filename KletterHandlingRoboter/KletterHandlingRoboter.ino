/*
 Name:		KletterHandlingRoboter.ino
 Created:	14.01.2017 08:37:39
 Author:	AüP Team 5
*/

// libary
/////////////////////////////////////////////////////////////////////

#include <IRremote.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <AccelStepper.h>
#include <Wire.h>
#include "TrackSensor.h"
#include "Timer.h"


// variables
/////////////////////////////////////////////////////////////////////

// const int
const int BaudRate = 9600;								// baud rate for serial communication

// int
int	Sequencer = 0;										// main sequence						

// enum
enum State {											// enumeration for sequencer
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

Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x60);
Adafruit_StepperMotor *StepperA1 = AFMS1.getStepper(200, 1);
Adafruit_StepperMotor *StepperA2 = AFMS1.getStepper(200, 2);
AccelStepper drive1(forwardstep1, backwardstep1);

// functions
/////////////////////////////////////////////////////////////////////

// stepper controll
void forwardstep1()
{
	StepperA1->onestep(FORWARD, DOUBLE);
	StepperA2->onestep(FORWARD, DOUBLE);
}
void backwardstep1()
{
	StepperA1->onestep(BACKWARD, DOUBLE);
	StepperA2->onestep(BACKWARD, DOUBLE);
}

// main sequencer
void MainSequence() {

	// variables
	/////////////////////////////////////////////////////////////////////

	// static bool
	static bool
		firstCycle = true,								// flag for first cycle
		SequencerTimerIN = false,						// input flag for SequencerTimer
		SequencerTimerOUT = false,						// output flag for SequencerTimer
		CenterPos = false,								// flag for FrontSensor is over center mark
		TopPos = false									// flag for top position
		;

	// bool
	bool
		SignalSensorFront = false,						// flag for result track sensor fornt
		SignalSensorCenter = false,						// flag for result track sensor center
		SignalSensorBack = false						// flag for result track sensor back
		;

	// static long
	static long
		SequencerTimerPt = 0,							// programmed time for SequencerTimer
		SequencerTimerEt = 0							// established time for SequencerTimer
		;
	
	// const int
	const int
		SensorFrontPin = 7,								// pin for front sensor
		SensorCenterPin = 8,							// pin for center sensor
		SensorBackPin = 9,								// pin for back sensor
		IRPin = 22										// pin for IR remote
		;

	// const unsigned long
	const unsigned long 
		IRRxStart = 16761405,							// start signal (  PLAY/PAUSE button )
		IRRxReset = 16736925,							// start signal ( CH button )
		StartupTime = 2000,								// startuptime [ms]
		WaitingTime = 5000,								// delay in center position [ms]
		GripperTime = 200,								// timer for gripper movement [ms]
		MoveOutTime = 2500,								// time to move pipe out [ms]
		TransportPosTime = 500,							// time to move arm in transport position [ms]
		HalfRotTime = 2000,								// time to rotate arm 180° [ms]
		AdjustmentValuePipeBottom = 200,				// time to adjust sensor position back [ms]
		AdjustmentValueTopPos = 1500,					// time to adjust sensor position front [ms]
		AdjustmentValueDragTop = 200					// time to adjust sensor position front [ms]
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
		IRRx.enableIRIn();								// start the IR receiver
	}

	// timer
	/////////////////////////////////////////////////////////////////////
	
	SequencerTimer.in(SequencerTimerIN);				// start timer		
	SequencerTimer.pt(SequencerTimerPt);				// programmed time
	SequencerTimerEt = SequencerTimer.et();				// established time
	SequencerTimerOUT = SequencerTimer.q();				// output

	// track sensors
	/////////////////////////////////////////////////////////////////////
	
	SignalSensorFront	= SensorFront.result();			// result signal front sensor
	SignalSensorCenter	= SensorCenter.result();		// result signal center sensor
	SignalSensorBack	= SensorBack.result();			// result signal back sensor
	

	// sequence
	/////////////////////////////////////////////////////////////////////

	Serial.println(Sequencer);

	switch (Sequencer) {
    
	// init robot
	case start:
		SequencerTimerPt = StartupTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			if (IRRx.decode(&Results))
			{
				if (IRRxStart == Results.value)
				{
					SequencerTimerIN = false;
					Sequencer = move_to_pipe_bottom;
				}
				IRRx.resume();
			}
		}
		break;

	// move to pipe bottom 
	case move_to_pipe_bottom:
		SequencerTimerPt = (AdjustmentValuePipeBottom);
		SequencerTimerIN = SignalSensorBack;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = grip_pipe_bottom;
		}
		break;

	// close gripper bottom position
	case grip_pipe_bottom:
		SequencerTimerPt = GripperTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = drag_pipe_out_bottom;
		}
		break;
		
	// drag pipe out bottom
	case drag_pipe_out_bottom:
		SequencerTimerPt = MoveOutTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_arm_in_transport_pos_to_top;
		}
		break;

	// pepare arm for move to top
	case move_arm_in_transport_pos_to_top:
			Sequencer = move_to_top;
		break;

	// move to top position
	case move_to_top:
		if (SignalSensorCenter)
		{
			CenterPos = true;
		}
		if (SignalSensorFront && CenterPos)
		{
			TopPos = true;
			CenterPos = false;
		}
		SequencerTimerPt = AdjustmentValueTopPos;
		SequencerTimerIN = TopPos;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			TopPos = false;
			Sequencer = move_arm_to_release_pos_top;
		}
		break;

	// move arm in release position on top
	case move_arm_to_release_pos_top:
		SequencerTimerPt = HalfRotTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = release_pipe_top;
		}
		break;

	// open gripper
	case release_pipe_top:
		SequencerTimerPt = GripperTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_to_center;
		}
		break;

	// move to center
	case move_to_center:
		if (SignalSensorCenter)
		{
			Sequencer = wait_5s;
		}
		break;

	// wait 5s
	case wait_5s:
		SequencerTimerPt = WaitingTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_to_pipe_top;
		}
		break;

	// move to pipe top position
	case move_to_pipe_top:
		if (SignalSensorFront)
		{
			TopPos = true;
		}
		SequencerTimerPt = AdjustmentValueDragTop;
		SequencerTimerIN = TopPos;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			TopPos = false;
			Sequencer = grip_pipe_top;
		}
		break;

	// close gripper on top position
	case grip_pipe_top:
		SequencerTimerPt = GripperTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = drag_pipe_out_top;
		}
		break;

	// move pipe out top
	case drag_pipe_out_top:
		SequencerTimerPt = MoveOutTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_arm_in_transport_pos_to_bottom;
		}
		break;

	// move arm in transport position to bottom
	case move_arm_in_transport_pos_to_bottom:
		SequencerTimerPt = TransportPosTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_to_bottom;
		}
		break;

	// move to bottom
	case move_to_bottom:
		if (SignalSensorCenter)
		{
			CenterPos = true;
		}
		if (SignalSensorBack && CenterPos)
		{
			CenterPos = false;
			Sequencer = release_pipe_bottom;
		}
		break;

	// open gripper
	case release_pipe_bottom:
		SequencerTimerPt = GripperTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_to_endpos;
		}
		break;

	// move to endposition
	case move_to_endpos:
		if (SignalSensorCenter)
		{
			Sequencer = end;
		}
		break;

	// end
	case end:
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
	static bool firstCycle = true;						// flag for first cycle

	// const int
	const int
		GripperServoPin = 9,							// pin for gripper servo
		ArmServoPin = 10,								// pin for arm servo
		ArmServoHome = 1573,							// home position for arm servo
		TransportPositionTop = 1500,					// arm position for transport to top
		ReleasePosTop = 1430,							// release position top
		GripperServoHome = 1150,						// home position for gripper servo
		GripperServoClosed = 900						// position gripper is closed
		;


	// objects
	/////////////////////////////////////////////////////////////////////


	static Servo GripperServo;
	static Servo ArmServo;

	// init
	/////////////////////////////////////////////////////////////////////

	if (firstCycle)
	{
		AFMS1.begin();									// start motor shield
		GripperServo.attach(GripperServoPin);			// attach GripperServo to pin 9
		ArmServo.attach(ArmServoPin);					// attach ArmServo to pin 10
		drive1.setMaxSpeed(500.0);						// set maximal speed
		drive1.setAcceleration(100.0);					// set acceleraton
	}	

	// arm servo
	/////////////////////////////////////////////////////////////////////
	
	// move arm servo in home position
	if (	(Sequencer == start)
		 ||	(Sequencer == move_arm_in_transport_pos_to_bottom))
	{
		ArmServo.writeMicroseconds(ArmServoHome);		
	}
	// move arm servo in transportposition
	if ((Sequencer == move_arm_in_transport_pos_to_top))
	{
		ArmServo.writeMicroseconds(TransportPositionTop);
	}
	//move arm servo in release position top
	if ((Sequencer == move_arm_to_release_pos_top))
	{
		ArmServo.writeMicroseconds(ReleasePosTop);
	}

	// gripper servo
	/////////////////////////////////////////////////////////////////////
	
	// move gripper in home position (opend)
	if (	(Sequencer == start)
		 ||	(Sequencer == release_pipe_top)
		 || (Sequencer == release_pipe_bottom))
	{
		GripperServo.writeMicroseconds(GripperServoHome);
	}
	// close gripper
	if (	(Sequencer == grip_pipe_bottom)
		 ||	(Sequencer == grip_pipe_top))
	{
		GripperServo.writeMicroseconds(GripperServoClosed);
	}


	// stepper
	/////////////////////////////////////////////////////////////////////

	// move forward

	if (	((Sequencer == start) && firstCycle)
		 ||	(Sequencer == move_to_top)
		 || (Sequencer == move_to_pipe_top)
		 || (Sequencer == move_to_endpos)
		 || (Sequencer == drag_pipe_out_bottom)
		 || (Sequencer == drag_pipe_out_top))
	{
		drive1.setAcceleration(300);
		drive1.setSpeed(-400);
		drive1.runSpeed();
	}
	// move backward
	if (	(Sequencer == move_to_pipe_bottom)
		 ||	(Sequencer == move_to_center)
		 || (Sequencer == move_to_bottom))
	{
		drive1.setAcceleration(300);
		drive1.setSpeed(400);
		drive1.runSpeed();
	}

	// reset first cycle variable
	/////////////////////////////////////////////////////////////////////

	firstCycle = false;						
}

// setup
void setup() {

	Serial.begin(BaudRate);								// start serial communication
	Sequencer = start;									// set sequence to start
}

// loop
void loop() {
	
	// call functions
	/////////////////////////////////////////////////////////////////////
	
	MainSequence();										// call main controll sequence
	Outputs();											// call outputs
}