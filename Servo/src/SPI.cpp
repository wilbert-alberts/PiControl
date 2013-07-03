/*
 * SPI.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: ben
 */

#include "SPI.h"
#include "BitBus.h"
#include "DigitalOut.h"
#include "DigitalIn.h"

#include "HAL.h"

#include <string>
#include <iostream>
#include <sys/time.h>

SPI::SPI() {
	bb = new BitBus(90);

	bb->createRegister(1, std::string("Height1"), 16, 0);
	bb->createRegister(2, std::string("Height2"), 16, 16);
	bb->createRegister(3, std::string("UBat"), 16, 32);
	bb->createRegister(4, std::string("EncPos"), 16, 48);

	bb->createRegister(11, std::string("PWM"), 16, 0);
	bb->createRegister(12, std::string("MotorDir"), 8, 16);

	//bb.createRegister();
	//bb.createRegister();
	//bb.createRegister();

	byteArray = new unsigned char[90 / 8];
	Pi2Mbed = DigitalOut::create(std::string("pi2mbed"), 16, 1);
	Mbed2Pi = DigitalIn::create("mbed2pi", 18);

}

SPI::~SPI() {
	delete[] byteArray;
}

void SPI::writeBus() {
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	try {
		waitOnSignal(Mbed2Pi, 0.0, 1000);
		std::cerr << "Copying to bytearray" << std::endl;
		bb->copyBytesTo(byteArray);

		std::cerr << "Set P2Mbed to 1" << std::endl;
		Pi2Mbed->set(1);
		std::cerr << "Wait for mbed to acknowlegde" << std::endl;
		waitOnSignal(Mbed2Pi, 1.0, 1000);

		std::cerr << "Initiating spi transfer" << std::endl;
		HAL::getInstance()->wiringPiSPIDataRW(0, byteArray, 8);
	} catch (int to) {
		std::cerr << "Timeout on SPI bus during write, reset" << std::endl;
		Pi2Mbed->set(1.0);
	}
}

void SPI::readBus() {
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	try {
		waitOnSignal(Mbed2Pi, 1.0, 1000);
		std::cerr << "Setting Pi2Mbed to 0" << std::endl;
		Pi2Mbed->set(0);
		std::cerr << "Waiting for mbed to acknowledge" << std::endl;
		waitOnSignal(Mbed2Pi, 0.0, 1000);

		std::cerr << "Initiating spi transfer" << std::endl;
		HAL::getInstance()->wiringPiSPIDataRW(0, byteArray, 8);

		std::cerr << "Copying bytes to array" << std::endl;
		bb->copyBytesFrom(byteArray);
	} catch (int to) {
		std::cerr << "Timeout on SPI bus read, reset" << std::endl;
		Pi2Mbed->set(1.0);
	}
}

void SPI::waitOnSignal(DigitalIn* in, double value, unsigned int timeoutInUs) {
	static struct timeval tv;
	static struct timeval to;

	gettimeofday(&to, 0);
	to.tv_sec = to.tv_sec + (to.tv_usec + timeoutInUs) / 1000000;
	to.tv_usec = to.tv_usec + (to.tv_usec + timeoutInUs) % 1000000;
	std::clog << "Waiting until: " << to.tv_sec << "," << to.tv_usec << std::endl;
	do {
		if (in->get() == value)
			return;

		gettimeofday(&tv, 0);
		std::clog << "Now: " << tv.tv_sec << "," << tv.tv_usec << std::endl;
	} while ((to.tv_sec > tv.tv_sec) || (to.tv_usec > tv.tv_usec));
	throw(1);
}

void SPI::writeBus(void* context) {
	static_cast<SPI*>(context)->writeBus();
}

void SPI::readBus(void* context) {
	static_cast<SPI*>(context)->readBus();
}

BitBus* SPI::getBB() {
	return bb;
}

