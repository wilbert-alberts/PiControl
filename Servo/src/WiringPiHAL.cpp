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
	int result;
	//::wiringPiSPIDataRW(channel, data, len);

	//static char myBuffer[len+8];
	static unsigned char myBuffer[80];

	frameBuffer(myBuffer, len+8);

	fillBuffer(myBuffer, data, len);

	//dumpBuffer("transmit", myBuffer, len+8);

	for (int i=0; i<len+8; i++) {
		result = ::wiringPiSPIDataRW(channel, myBuffer+i, 1);
		if (result<0)
			std::clog << "SPI communication error: " << result << std::endl;
	}

	//dumpBuffer("received", myBuffer, len+8);

	captureBuffer(myBuffer, data, len);

}

void WiringPiHAL::frameBuffer(unsigned char *data, int len)
{
	data[0] = 0xAA;
	data[1] = 0x55;
	data[2] = 0xAA;
	data[3] = 0x55;

	data[len-4] = 0x55;
	data[len-3] = 0xaa;
	data[len-2] = 0x55;
	data[len-1] = 0xaa;
}

void WiringPiHAL::fillBuffer(unsigned char* myBuffer, unsigned char* source, int len)
{
	for (int i=0; i<len; i++)
		myBuffer[4+i] = 2*i+1;
		//myBuffer[4+i] = source[i];
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
