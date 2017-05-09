#include  "Timer.h"


Timer::Timer()
	: startzeit(millis())
{
}

Timer::~Timer()
{
}

void Timer::in(bool in)
{
	in_ = in;
}

bool Timer::q()
{
}

void Timer::pt(unsigned long pt)
{
	pt_ = pt;
}

unsigned long Timer::et()
{
	return et_;
}


// TON
TON::TON()
	: Timer()
{
}

TON::~TON()
{
}

bool TON::q()
{
	et_ = millis() - startzeit;

	if (!in_)
	{
		et_ = 0;
		q_  = in_;
		startzeit = millis();
	} else if (in_ && !q_)
	{
		if (et_ >= pt_){
			q_ = in_;
		}
	}
	
	return q_;
}


// TOF
TOF::TOF()
	: Timer()
{
}

TOF::~TOF()
{
}

bool TOF::q()
{
	et_ = startzeit - millis();
	
	if (in_)
	{
		et_ = 0;
		q_ = in_;
		startzeit = millis() + pt_;
	}
	else if (!in_ && q_)
	{
		if (et_ <= 0) {
			q_ = in_;
		}
	}

	return q_;
}
