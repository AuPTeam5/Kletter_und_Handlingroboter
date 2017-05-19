/*
Name:		Timer.h
Created:	08.05.2017 15:44:00
Author:		Matthias Stieger
*/

/*

---description---

static TON ton_Timer;						--> creates a timer object TON = on delay, TOF = off delay

ton_Timer.in();								--> .in() starts the timer				e.g. ton_Timer.in(digitalRead(timerPin))
ton_Timer.pt();								--> .pt() programmed time in [ms]		e.g. ton_Timer.pt(5000)
ton_Timer.et();								--> .et() established time in [ms]		e.g. ton_Timer.et(2500) after 2.5 sec
ton_Timer.q());								--> .q() output on or off if et >= pt

*/


#ifndef Timer_h
#define Timer_h

#include <Arduino.h>


class Timer											// base class for TON and TOF
{
public:
	Timer();										// constructor
	virtual ~Timer();								// destructor

	virtual void in(bool in);						// start public
	virtual bool q();								// out public
	virtual void pt(unsigned long pt);				// programmed time public
	virtual unsigned long et();						// established time public

	bool lastq = false;								// last state 
	unsigned long startzeit;						// start time

protected:
	bool in_ = false;								// start protected
	bool q_ = false;								// out protected
	unsigned long pt_ = 0;							// programmed time protected
	unsigned long et_ = 0;							// established time protected
};


class TON : public Timer							// derived function of timer
{
public:
	  TON();										// constructor
	  virtual ~TON();								// destructor

	  virtual bool q();								// returns state TON
};


class TOF : public Timer							// derived function of timer
{
public:
	TOF();											// constructor
	virtual ~TOF();									// destructor

	virtual bool q();								// returns state TOF
};

#endif