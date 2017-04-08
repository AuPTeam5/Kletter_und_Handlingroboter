/*
Name:		TrackSensor.cpp
Created:	08.04.2017 15:44:00
Author:		Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#include "Arduino.h"
#include "TrackSensor.h"

TrackSensor::TrackSensor(int pin){
	_pin = pin;
}

boolean TrackSensor::result() {
	
	// private
	const long timer = 50;
	int mapvalue = 0;
	unsigned long average = 0;
	unsigned long Reference = 0;
	unsigned long CurrentTime = millis();

	if ((CurrentTime - Reference) >= timer ) {
		Reference = CurrentTime;
		for (int i = 0; i <= 9; i++) {
			average += analogRead(_pin);
		}
		mapvalue = map((average/10), 0, 1024, 0, 100);
		if (mapvalue <= 50) {
			return true;
		}
		else {
			return false;
		}		
	}
}
