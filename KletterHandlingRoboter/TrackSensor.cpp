/*
Name:		TrackSensor.cpp
Created:	08.04.2017 15:44:00
Author:		Matthias Stieger
*/

#include "Arduino.h"
#include "TrackSensor.h"

TrackSensor::TrackSensor(int pin)
	: _pin(pin), average(1)
{
	_pin = pin;
}
TrackSensor::~TrackSensor()
{
}


bool TrackSensor::result() {
	
	int mapvalue = 0;
	unsigned long average = 0;
	
	for (int i = 0; i <= 9; i++) 
	{
		average += analogRead(_pin);
	}

	mapvalue = map((average/10), 0, 1024, 0, 100);

	if ((mapvalue > 10) && (mapvalue < 60))
	{
		return true;
	}
	else 
	{
		return false;
	}
}
