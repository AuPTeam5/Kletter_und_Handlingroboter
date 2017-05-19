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
const int BaudRate = 9600;								// baud rate for serial communication

// int
int	Sequencer = 0;										// main sequence						

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
		firstCycle = true,								// flag for first cycle
		SequencerTimerIN = false,						// input flag for SequencerTimer
		SequencerTimerOUT = false,						// output flag for SequencerTimer
		CenterPos = false								// flag for FrontSensor is over center mark
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
		SensorFrontPin = 0,								// pin for front sensor
		SensorCenterPin = 1,							// pin for center sensor
		SensorBackPin = 2,								// pin for back sensor
		IRPin = 7										// pin for IR remote
		;

	// const unsigned long
	const unsigned long 
		IRRxStart = 16761405,							// start signal (  PLAY/PAUSE button )
		IRRxReset = 16736925,							// start signal ( CH button )
		StartupTime = 2000,								// startuptime [ms]
		WaitingTime = 5000,								// delay in center position [ms]
		GripperTime = 2000,								// timer for gripper movement [ms]
		MoveOutTime = 3000,								// time to move pipe out [ms]
		TransportPosTime = 3000,						// time to move arm in transport position [ms]
		HalfRotTime = 3000								// time to rotate arm 180° [ms]
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
    
	// init
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

	// move to pipe bottom position
	case move_to_pipe_bottom:
		if (SignalSensorBack)
		{
			Sequencer = grip_pipe_bottom;
		}
		break;

	// close gripper bottom pos
	case grip_pipe_bottom:
		SequencerTimerPt = GripperTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = drag_pipe_out_bottom;
		}
		break;
		
	// move pipe out bottom
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
		SequencerTimerPt = TransportPosTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_to_top;
		}
		break;

	// move to top position
	case move_to_top:
		if (SignalSensorCenter)
		{
			CenterPos = true;
		}
		if (SignalSensorFront && CenterPos)
		{
			CenterPos = false;
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
			Sequencer = grip_pipe_top;
		}
		break;

	// close gripper on top pos
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

	// move arm in transport pposition to bottom
	case move_arm_in_transport_pos_to_bottom:
		SequencerTimerPt = HalfRotTime;
		SequencerTimerIN = true;
		if (SequencerTimerOUT)
		{
			SequencerTimerIN = false;
			Sequencer = move_to_bottom;
		}
		break;

	// move to bottom
	case move_to_bottom:
		if (SignalSensorBack)
		{
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
	static bool firstCycle = true;

	// const int
	const int
		GripperServoPin = 9,							// pin for gripper servo
		ArmServoPin = 10,								// pin for arm servo
		ShieldAdress = 0x60,							// motor shield adress
		StepperResolution = 200,						// steps / u ( 360° / 1.8° per step)
		StepperSpeed = 180,								// stepper speed [rpm]
		ArmServoHome = 1573,							// home position for arm servo
		TransportPositionTop = 1500,					// arm position for transport to top
		ReleasePosTop = 1438,							// release position top
		GripperServoHome = 900,							// home position for gripper servo
		GripperServoClosed = 900,						// position gripper is closed
		PipeOutDistance = 200							// steps to move pipe out
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
		AFMS.begin();									// start motor shield
		GripperServo.attach(GripperServoPin);			// attach GripperServo to pin 9
		ArmServo.attach(ArmServoPin);					// attach ArmServo to pin 10
		Stepper->setSpeed(StepperSpeed);				// set stepper speed
	}	

	// arm servo
	/////////////////////////////////////////////////////////////////////
	

	if (	(Sequencer == start)
		 ||	(Sequencer == move_arm_in_transport_pos_to_bottom))
	{
		ArmServo.writeMicroseconds(ArmServoHome);
	}
	if ((Sequencer == move_arm_in_transport_pos_to_top))
	{
		ArmServo.writeMicroseconds(TransportPositionTop);
	}
	if ((Sequencer == move_arm_to_release_pos_top))
	{
		ArmServo.writeMicroseconds(ReleasePosTop);
	}

	// gripper servo
	/////////////////////////////////////////////////////////////////////
	
	if (	(Sequencer == start)
		 ||	(Sequencer == release_pipe_top)
		 || (Sequencer == release_pipe_bottom))
	{
		GripperServo.writeMicroseconds(GripperServoHome);
	}
	if (	(Sequencer == grip_pipe_bottom)
		 ||	(Sequencer == grip_pipe_top))
	{
		GripperServo.writeMicroseconds(GripperServoClosed);
	}


	// stepper
	/////////////////////////////////////////////////////////////////////

	if (	((Sequencer == start) && firstCycle)
		 ||	(Sequencer == move_to_top)
		 || (Sequencer == move_to_pipe_top)
		 || (Sequencer == move_to_endpos))
	{
		Stepper->onestep(FORWARD, DOUBLE);
	}
	if (	(Sequencer == move_to_pipe_bottom)
		 ||	(Sequencer == move_to_center)
		 || (Sequencer == move_to_bottom))
	{
		Stepper->onestep(BACKWARD, DOUBLE);
	}
	if (	(Sequencer == drag_pipe_out_bottom)
		 ||	(Sequencer == drag_pipe_out_top))
	{
		Stepper->step(PipeOutDistance, FORWARD, DOUBLE);
	}

	// reset first cycle variable
	/////////////////////////////////////////////////////////////////////

	firstCycle = false;
}

// setup
void setup() {

	Serial.begin(BaudRate);								// start serial communication
	Sequencer = start;									// set sequence to init
}

// loop
void loop() {
	
	// call functions
	/////////////////////////////////////////////////////////////////////
	
	MainSequence();										// main controll sequence
	Outputs();											// outputs
}