/*
 * HAL.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: wilbert
 */

#include "WiringPiHAL.h"

#define REALMODE

#ifdef REALMODE

#include <iostream>
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

	static unsigned char framedData[80];

	frameBuffer(framedData, len);
	fillBuffer(framedData, data, len);

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

	captureBuffer(framedData, data, len);

}

void WiringPiHAL::frameBuffer(unsigned char *data, int len)
{
	data[0] = 0xAA;
	data[1] = 0x55;
	data[2] = 0xAA;
	data[3] = 0x55;

	data[len+4] = 0x55;
	data[len+5] = 0xaa;
	data[len+6] = 0x55;
	data[len+7] = 0xaa;
}

void WiringPiHAL::fillBuffer(unsigned char* myBuffer, unsigned char* source, int len)
{
	for (int i=0; i<len; i++)
		//myBuffer[4+i] = 2*i+1;
		myBuffer[4+i] = source[i];
}

void WiringPiHAL::captureBuffer(unsigned char* myBuffer, unsigned char* dest, int len)
{
	if ((myBuffer[0] != 0xaa) ||
			(myBuffer[1] != 0x55) ||
			(myBuffer[2] != 0xaa) ||
			(myBuffer[3] != 0x55) ||
			(myBuffer[8+len-4] != 0x55) ||
			(myBuffer[8+len-3] != 0xaa) ||
			(myBuffer[8+len-2] != 0x55) ||
			(myBuffer[8+len-1] != 0xaa)) {
		std::clog << "SPI communication error: incomplete/corrupt frame" << std::endl;
		dumpBuffer("captured: ", myBuffer, len+8);
	}

	for (int i=0; i<len; i++)
		dest[i] = myBuffer[4+i];

/*	In case of unreliable spi communication
	for (int i=0; i<4; i++) {
		if ((myBuffer[i] == 0xAA) &&
			(myBuffer[i+1] == 0x55) &&
			(myBuffer[2+i+len] == 0x55) &&
			(myBuffer[2+i+1+len] == 0xAA)) {
			for (int j=0; j<len; j++) {
				data[j] = myBuffer[i+2+j];
			}
			return;
		}
	}
*/
}

void WiringPiHAL::dumpBuffer(const char* msg, unsigned char* myBuffer, int len)
{
	printf("%s: ", msg);
	for (int i=0; i<len; i++) {
		printf(" %02X", myBuffer[i]);
	}
	printf("\n");
}

#endif
