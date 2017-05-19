/*
Name:		TrackSensor.h
Created:	08.04.2017 15:44:00
Author:		Matthias Stieger
*/

#ifndef TrackSensor_h
#define TrackSensor_h

class TrackSensor {						// class TrackSensor

	public:
		TrackSensor(int pin);			// constructor 
		virtual ~TrackSensor();			// destructor
		bool result();					// returns sensor signal as bool

	private:
		int _pin;						// private pin number
		int mapvalue;					// mapped sensor value
		unsigned long average;			// variable for average calculation
};

#endif