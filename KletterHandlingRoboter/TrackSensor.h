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
		virtual ~TrackSensor();
		bool result();

	private:
		int _pin;
		int mapvalue;
		unsigned long average;
};

#endif

