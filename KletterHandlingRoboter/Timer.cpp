/*
Name:		Timer.cpp
Created:	08.05.2017 15:44:00
Author:		Matthias Stieger
*/

#include  "Timer.h"


Timer::Timer()										// constructor for Timer
	: startzeit(millis())							// copy actual time in startzeit
{
}

Timer::~Timer()										// destructor for Timer
{
}

void Timer::in(bool in)								// copy input function
{
	in_ = in;
}

bool Timer::q()										
{
}

void Timer::pt(unsigned long pt)					// copy programmed time
{
	pt_ = pt;
}

unsigned long Timer::et()							// write established time
{
	return et_;
}


// TON
TON::TON()											// constructor for TON
	: Timer()
{
}

TON::~TON()											// destructor for TON
{
}

bool TON::q()										// TON function
{
	et_ = millis() - startzeit;						// calculate established time

	if (!in_)										// check in_  
	{
		et_ = 0;									// set et to 0
		q_  = in_;									// write in_ to q_
		startzeit = millis();						// write millis in startzeit
	} else if (in_ && !q_)							// check in_ and q_ if !in_
	{
		if (et_ >= pt_){							// check if et_ >= pt_
			q_ = in_;								// write in_ in q_
		}
	}
	
	return q_;										// returns state of q_
}


// TOF
TOF::TOF()											// constructor for TOF
	: Timer()
{
}

TOF::~TOF()											// destructor for TON
{	
}

bool TOF::q()										// TOF function
{
	et_ = pt_ - (startzeit - millis());				// calculate established time
	
	if (in_)										// check in_
	{
		et_ = 0;									// set et to 0
		q_ = in_;									// write in_ to q_
		startzeit = millis() + pt_;					// write millis + programmed time in startzeit
	}
	else if (!in_ && q_)							// check ^!in_ and q_ if in_
	{
		if (et_ >= pt_) {							// check if et_ >= pt_
			q_ = in_;								// write in_ in q_
		}
	}

	return q_;										// returns state of q_
}