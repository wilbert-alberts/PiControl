/*
 * HAL.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: wilbert
 */

#include "WiringPiHAL.h"

#include <stdexcept>
#include <wiringPi.h>
#include <wiringPiSPI.h>

void WiringPiHAL::registerHAL() {
	HAL::registerHAL(new WiringPiHAL());
}

void WiringPiHAL::setup() {
	wiringPiSetup();
	wiringPiSPISetup(0, 1000000); // Note: channel doesn't matter
}

void WiringPiHAL::pinMode(int pin, int mode) {
	if (mode == HAL::IN) {
		pinMode(pin, INPUT);
		return;
	}

	if (mode == HAL::OUT) {
		pinMode(pin, OUTPUT);
		return;
	}
	throw std::out_of_range("Illegal pinMode");
}

int WiringPiHAL::digitalRead(int pin) {
	return digitalRead(pin);
}
void WiringPiHAL::digitalWrite(int pin, int value) {
	digitalWrite(pin, value);
}
void WiringPiHAL::wiringPiSPIDataRW(int channel, unsigned char *data, int len) {
	wiringPiSPIDataRW(channel, data, len);
}
