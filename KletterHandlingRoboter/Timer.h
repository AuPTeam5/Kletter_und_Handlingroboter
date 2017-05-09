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
