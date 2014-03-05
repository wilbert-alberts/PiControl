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

SPI* SPI::instance = 0;

SPI* SPI::getInstance()
{
	if (instance==0)
		instance = new SPI();
	return instance;
}

SPI::SPI() {
	enabled = new Parameter(std::string("SPI.enabled"), 0.0);
	bb = new BitBus();

	createRegister(HEIGHT1, std::string("SPI.Height1"),    0, 16);
	createRegister(HEIGHT2, std::string("SPI.Height2"),   16, 16);

	createRegister(UBAT, std::string("SPI.UBat"),   32, 16);

	createRegister(GYRO, std::string("SPI.Gyro"),      48, 16);
	createRegister(ENCPOS, std::string("SPI.EncPos"),    64, 16);

	createRegister(PWM, std::string("SPI.PWM"),      80, 16);
	createRegister(MOTORDIR, std::string("SPI.MotorDir"), 96,  8);

	createRegister(OVERSAMPLING, std::string("SPI.oversampling"), 104,  8);

	createRegister(SAMPLESTAKEN, std::string("SPI.samplesTaken"), 112,  8);

	Pi2Mbed = DigitalOut::create(std::string("SPI.pi2mbed"), 4, 1);
	Mbed2Pi = DigitalIn::create("SPI.mbed2pi", 5);
}

SPI::~SPI() {
	delete[] byteArray;
}


void SPI::createRegister(int id, const std::string& n, int start, int length)
{
	bb->createRegister(id, n, start, length);
	registers[id]=new Parameter(n,0.0);

	int nb = (start+length)/8 + ((start+length)%8==0 ? 0 : 1);
	if (nb > nrBytes) {
		unsigned char* newBytes = new unsigned char[nb];
		std::copy(byteArray, byteArray+nrBytes, newBytes);
		delete[] byteArray;
		byteArray = newBytes;
		nrBytes = nb;
	}
	std::clog << "Adding " << n << " nrBytes: " << nrBytes << std::endl;
}

bool SPI::isEnabled()
{
	if (bEnabled && *enabled==0.0) {
		// SPI got disabled
		Mbed2Pi->setEnabled(false);
		Pi2Mbed->setEnabled(false);
		bEnabled = false;
		return bEnabled;
	}

	if (!bEnabled && *enabled != 0.0) {
		// SPI got enabled
		Mbed2Pi->setEnabled(true);
		Pi2Mbed->setEnabled(true);
		bEnabled = true;
		return bEnabled;
	}

	// In case of no change
	return bEnabled;
}

void SPI::writeBus() {
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	//std::clog << "SPI::writeBus"<< std::endl;
	copyFromParameters();

	try {
		//std::clog << "Verifying state of Mbed2Pi line" << std::endl;
		waitOnSignal(Mbed2Pi, 0.0, 100000);

		//std::clog << "Set P2Mbed to 1" << std::endl;
		Pi2Mbed->set(1);

		//std::clog << "Wait for mbed to acknowlegde" << std::endl;
		waitOnSignal(Mbed2Pi, 1.0, 100000);

		//std::clog << "Initiating spi transfer" << std::endl;
		if (isEnabled())
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

		//std::clog <<"setting register " << p->getName() << " to value: " << *p << std::endl;
		bb->setRegister(byteArray, id, *p);
	}
}

void SPI::copyToParameters()
{
//	for (int i=0; i<nrBytes; i++) {
//		printf ("0x%02x ", byteArray[i]);
//	}
//	printf("\n");
	for (auto iter = registers.begin(); iter!= registers.end(); iter++) {
		Parameter* p = iter->second;
		int id = iter->first;

		*p = bb->getRegister(byteArray, id);
	}

}
void SPI::readBus() {
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	//std::clog << "SPI::readBus"<< std::endl;
	try {
		//std::clog << "Verifying the Mbed2Pi line"<< std::endl;
		waitOnSignal(Mbed2Pi, 1.0, 100000);

		//std::clog << "Setting Pi2Mbed to 0" << std::endl;
		Pi2Mbed->set(0);

		//std::clog << "Waiting for mbed to acknowledge" << std::endl;
		waitOnSignal(Mbed2Pi, 0.0, 100000);

		//std::clog << "Initiating spi transfer" << std::endl;
		if (isEnabled())
			HAL::getInstance()->wiringPiSPIDataRW(0, byteArray, nrBytes);

		//std::clog << "Copying bytes to parameters" << std::endl;
		//for (int i=0; i<nrBytes; i++) byteArray[i] = 0;
		//std::clog << "low byte: " <<  (int)byteArray[6] << std::endl;
		//std::clog << "high byte: " << (int)byteArray[7] << std::endl;
		//byteArray[7] =2;

		copyToParameters();

	} catch (int to) {
		std::cerr << "Timeout on SPI bus read, reset" << std::endl;
		Pi2Mbed->set(1.0);
	}
}

void SPI::waitOnSignal(DigitalIn* in, double value, unsigned int timeoutInUs) {
	static struct timeval tv;
	static struct timeval to;

	gettimeofday(&tv, 0);
	to.tv_sec = tv.tv_sec + ((tv.tv_usec + timeoutInUs) / 1000000);
	to.tv_usec = (tv.tv_usec + timeoutInUs) % 1000000;
	do {
		// In case not enabled, return immediately
		if (!isEnabled() || (in->get() == value))
			return;

		gettimeofday(&tv, 0);
	} while ((to.tv_sec > tv.tv_sec) || (to.tv_usec > tv.tv_usec));
	throw(1);
}

void SPI::writeBus(void* context) {
	static_cast<SPI*>(context)->writeBus();
}

void SPI::readBus(void* context) {
	static_cast<SPI*>(context)->readBus();
}

double SPI::getRegister(int reg)
{
	//TODO: check whether 'get' functionality should be based
	//      on registers or parameters
	return bb->getRegister(byteArray, reg);
}

void SPI::setRegister(int reg, double value)
{
	bb->setRegister(byteArray, reg, value);

	Parameter* p = registers[reg];
	*p = value;

}
