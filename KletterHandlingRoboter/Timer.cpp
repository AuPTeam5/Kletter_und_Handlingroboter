#include  "Timer.h"

Timer::Timer()
{
    lastq = _q;
    startzeit = millis();
}

void Timer::in(bool& in)
{
	_in = in;
}

void Timer::et(unsigned long& et)
{
	et = _et;
}

void Timer::pt(unsigned long& pt)
{
	_pt = pt;
}

void Timer::q(bool& q)
{
	_et = millis()-startzeit;
	if(!_in) 
	{
		_et = 0;
		_q  = _in;
		startzeit  = millis();
	}else if(_in && !_q) 
	{
		if(_et >= _pt){
			_q = _in;
		}
	}
	q = _q;
}