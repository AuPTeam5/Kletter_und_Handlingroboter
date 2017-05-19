/*
Name:		TrackSensor.cpp
Created:	08.04.2017 15:44:00
Author:		Matthias Stieger
*/

#include "Arduino.h"
#include "TrackSensor.h"

TrackSensor::TrackSensor(int pin)						// constructor for TrackSensor
	: _pin(pin), average(0)								// init _pin with pin and average with 1
{
}
TrackSensor::~TrackSensor()								// destructor for TrackSensor
{
}


bool TrackSensor::result() {							// convert the analog input in a digital signal
	
	int mapvalue = 0;									// set mapvalue to 0					
	unsigned long average = 0;							// set average to 0
	
	for (int i = 0; i <= 9; i++)						
	{
		average += analogRead(_pin);					// read input 10 times
	}

	mapvalue = map((average/10), 0, 1024, 0, 100);		// map the value 0 to 100 

	if ((mapvalue > 10) && (mapvalue < 60))				// return true if mapped value between 10 and 60
	{
		return true;
	}
	else 
	{
		return false;
	}
}
