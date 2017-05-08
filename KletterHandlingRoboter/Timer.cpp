#include  "Timer.h"

// TON

TON::TON()
{
    lastq = _q;
    startzeit = millis();
}

void TON::in(bool& in)
{
	_in = in;
}

void TON::et(unsigned long& et)
{
	et = _et;
}

void TON::pt(unsigned long& pt)
{
	_pt = pt;
}

void TON::q(bool& q)
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

// TOF

TOF::TOF()
{
	lastq = _q;
	startzeit = millis();
}

void TOF::in(bool& in)
{
	_in = in;
}

void TOF::et(unsigned long& et)
{
	et = _et;
}

void TOF::pt(unsigned long& pt)
{
	_pt = pt;
}

void TOF::q(bool& q)
{
	_et = startzeit - millis();
	if (_in)
	{
		_et = 0;
		_q = _in;
		startzeit = millis() + _pt;
	}
	else if (!_in && _q)
	{
		if (_et <= 0) {
			_q = _in;
		}
	}
	q = _q;
}