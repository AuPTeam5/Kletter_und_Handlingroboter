/*
Name:		TrackSensor.h
Created:	08.04.2017 15:44:00
Author:		Matthias Stieger
*/

#ifndef TrackSensor_h
#define TrackSensor_h

class TrackSensor {

	public:
		TrackSensor(int pin);
		bool result();

	private:
		int _pin;
		int timer;
		int mapvalue;
		unsigned long average;
		unsigned long Reference;
		unsigned long CurrentTime;
};

#endif

