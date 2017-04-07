/*
CheckSensor.cpp - used for reading an anloge track sensor
Created by Matthias Stieger, 07.04.2017
*/

#include "Arduino.h"
#include "TrackSensor.h"

TrackSensor::TrackSensor(int pin){
	_pin = pin;
}

boolean TrackSensor::result() {
	
	// private
	const long interval = 500;
	int mapvalue = 0;
	unsigned long Reference = 0;
	unsigned long CurrentTime = millis();

	if ((CurrentTime - Reference) >= interval) {
		mapvalue = map(analogRead(_pin), 0, 1024, 0, 100);
		if (mapvalue <= 50) {
			return true;
		}
		else {
			return false;
		}
		Reference = CurrentTime;
	}
}