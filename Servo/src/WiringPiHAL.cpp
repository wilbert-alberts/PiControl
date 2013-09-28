/*
 * HAL.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: wilbert
 */

#include "WiringPiHAL.h"

#ifdef REALMODE

#include <stdexcept>
#include <wiringPi.h>
#include <wiringPiSPI.h>

void WiringPiHAL::registerHAL() {
	HAL::registerHAL(new WiringPiHAL());
}

void WiringPiHAL::setup() {
	wiringPiSetup();
	wiringPiSPISetup(0, 500000); // Note: channel doesn't matter
}

void WiringPiHAL::pinMode(int pin, int mode) {
	if (mode == HAL::IN) {
		::pinMode(pin, INPUT);
		return;
	}

	if (mode == HAL::OUT) {
		::pinMode(pin, OUTPUT);
		return;
	}
	throw std::out_of_range("Illegal pinMode: ");
}

int WiringPiHAL::digitalRead(int pin) {
	return ::digitalRead(pin);
}
void WiringPiHAL::digitalWrite(int pin, int value) {
	::digitalWrite(pin, value);
}

void WiringPiHAL::wiringPiSPIDataRW(int channel, unsigned char *data, int len) {

	// Run protocol to get rid of remaining data in SPI FIFO
	typedef enum {
		INIT,
		HALFWAY,
		EXIT
	} State_enum;

	State_enum state = INIT;
	unsigned char buffer;
	do {
		switch (state) {
		case INIT:
			buffer = 0xAA;
			::wiringPiSPIDataRW(channel, &buffer, 1);
			switch (buffer) {
			case 0xAA:
				state = HALFWAY;
				break;
			default:
				state = INIT;
				break;
			}
			break;
		case HALFWAY:
			buffer = 0xBB;
			::wiringPiSPIDataRW(channel, &buffer, 1);
			switch (buffer) {
			case 0xAA:
				state = HALFWAY;
				break;
			case 0xBB:
				state = EXIT;
				break;
			default:
				state = INIT;
				break;
			}
			break;
		default:
			// Should not be reached
			std::clog << "This code should not be reached: " << std::endl;
			break;
		}
	} while (state != EXIT);

	// MBED en PI in sync, no communicate buffer.
	for (int i=0; i<len; i++) {
		::wiringPiSPIDataRW(channel, data+i, 1);
	}

}



#endif
