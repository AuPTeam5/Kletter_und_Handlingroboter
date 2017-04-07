/*
 Name:		KletterHandlingRoboter.ino
 Created:	14.01.2017 08:37:39
 Author:	Florian Steiger, Kushtrim Thaqi, Matthias Stieger
*/

#include "TrackSensor.h"

TrackSensor Sensor1(0);
TrackSensor Sensor2(1);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.println("Senor1 = " + Sensor1.result());
	Serial.println("Senor2 = " + Sensor2.result());
}
