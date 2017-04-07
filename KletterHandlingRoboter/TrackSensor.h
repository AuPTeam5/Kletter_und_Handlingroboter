/*
TrackSensor.h - used for read an anloge track sensor
Created by Matthias Stieger, 07.04.2017
*/

#ifndef TrackSensor_h
#define TrackSensor_h

class TrackSensor {

	public:
		TrackSensor(int pin);
		boolean result();

	private:
		int _pin;
		const long interval;
		int mapvalue;
		unsigned long Reference;
		unsigned long CurrentTime;
};

#endif

