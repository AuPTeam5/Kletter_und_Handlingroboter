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


class Timer
{
public:
	Timer();
	virtual ~Timer();

	virtual void in(bool in);
	virtual bool q();
	virtual void pt(unsigned long pt);
	virtual unsigned long et();

	bool lastq = false;
	unsigned long startzeit;

protected:
	bool in_ = false;
	bool q_ = false;
	unsigned long pt_ = 0;
	unsigned long et_ = 0;
};


class TON : public Timer
{
public:
	  TON();
	  virtual ~TON();

	  virtual bool q();
};


class TOF : public Timer
{
public:
	TOF();
	virtual ~TOF();

	virtual bool q();
};

#endif