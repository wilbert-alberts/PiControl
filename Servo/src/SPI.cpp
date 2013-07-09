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
#include "Parameter.h"
#include "HAL.h"

#include <string>
#include <iostream>
#include <sys/time.h>
#include <algorithm>

SPI::SPI() {
	// 90 bits. n+7/8 results in number of bytes.
	bb = new BitBus();

	createRegister(1, std::string("SPI.Height1"),    0, 16);
	createRegister(2, std::string("SPI.Height2"),   16, 16);
	createRegister(3, std::string("SPI.UBat"),      32, 16);
	createRegister(4, std::string("SPI.EncPos"),    48, 16);

	createRegister(11, std::string("SPI.PWM"),      64, 16);
	createRegister(12, std::string("SPI.MotorDir"), 72,  8);

	Pi2Mbed = DigitalOut::create(std::string("pi2mbed"), 4, 1);
	Mbed2Pi = DigitalIn::create("mbed2pi", 5);
}

SPI::~SPI() {
	delete[] byteArray;
}

void SPI::createRegister(int id, const std::string& n, int start, int length)
{
	bb->createRegister(id, n, start, length);
	registers[id]=new Parameter(n);

	int nb = (start+length)/8 + ((start+length)%8==0 ? 0 : 1);
	if (nb > nrBytes) {
		unsigned char* newBytes = new unsigned char[nb];
		std::copy(byteArray, byteArray+nrBytes, newBytes);
		delete[] byteArray;
		byteArray = newBytes;
		nrBytes = nb;
	}

}
void SPI::writeBus() {
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	std::clog << "SPI::writeBus"<< std::endl;
	copyFromParameters();

	try {
		std::clog << "Verifying state of Mbed2Pi line" << std::endl;
		waitOnSignal(Mbed2Pi, 0.0, 1000);

		std::clog << "Set P2Mbed to 1" << std::endl;
		Pi2Mbed->set(1);

		std::clog << "Wait for mbed to acknowlegde" << std::endl;
		waitOnSignal(Mbed2Pi, 1.0, 1000);

		std::clog << "Initiating spi transfer" << std::endl;
		HAL::getInstance()->wiringPiSPIDataRW(0, byteArray, nrBytes);
	} catch (int to) {
		std::cerr << "Timeout on SPI bus during write, reset" << std::endl;
		Pi2Mbed->set(1.0);
	}
}

void SPI::copyFromParameters()
{
	for (auto iter = registers.begin(); iter!= registers.end(); iter++) {
		Parameter* p = iter->second;
		int id = iter->first;

		bb->setRegister(byteArray, id, p->get());
	}
}

void SPI::copyToParameters()
{
	for (auto iter = registers.begin(); iter!= registers.end(); iter++) {
		Parameter* p = iter->second;
		int id = iter->first;

		p->set(bb->getRegister(byteArray, id));
	}

}
void SPI::readBus() {
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	std::clog << "SPI::readBus"<< std::endl;
	try {
		std::clog << "Verifying the Mbed2Pi line"<< std::endl;
		waitOnSignal(Mbed2Pi, 1.0, 1000);

		std::clog << "Setting Pi2Mbed to 0" << std::endl;
		Pi2Mbed->set(0);

		std::clog << "Waiting for mbed to acknowledge" << std::endl;
		waitOnSignal(Mbed2Pi, 0.0, 1000);

		std::clog << "Initiating spi transfer" << std::endl;
		HAL::getInstance()->wiringPiSPIDataRW(0, byteArray, nrBytes);

		std::clog << "Copying bytes to parameters" << std::endl;
		copyToParameters();

	} catch (int to) {
		std::cerr << "Timeout on SPI bus read, reset" << std::endl;
		Pi2Mbed->set(1.0);
	}
}

void SPI::waitOnSignal(DigitalIn* in, double value, unsigned int timeoutInUs) {
	static struct timeval tv;
	static struct timeval to;

#ifdef REALMODE
	gettimeofday(&tv, 0);
	to.tv_sec = tv.tv_sec + ((tv.tv_usec + timeoutInUs) / 1000000);
	to.tv_usec = (tv.tv_usec + timeoutInUs) % 1000000;
	do {
		if (in->get() == value)
			return;

		gettimeofday(&tv, 0);
	} while ((to.tv_sec > tv.tv_sec) || (to.tv_usec > tv.tv_usec));
	throw(1);
#endif
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

