/*
 * SPI.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: ben
 */

#include "SPI.h"
#include "DigitalOut.h"
#include "DigitalIn.h"
#include "Parameter.h"
#include "HAL.h"

#include <string>
#include <iostream>
#include <sys/time.h>
#include <algorithm>

#include <cstdint>



SPI::SPI(ServoModule* wrapped)
: ServoModule("SPI", wrapped)
{
	createRegister16(SPI::HEIGHT1, "height1", &buffer.height1);
	createRegister16(SPI::HEIGHT2, "height2", &buffer.height2);
	createRegister16(SPI::UBAT, "ubat", &buffer.ubat);
	createRegister16(SPI::GYRO, "gyro", &buffer.gyro);
	createRegister16(SPI::ENCPOS, "encpos", &buffer.encpos);
	createRegister16(SPI::PWM, "pwm", &buffer.pwm);

	createRegister8(SPI::MOTORDIR, "motordir", &buffer.motordir);
	createRegister8(SPI::OVERSAMPLING, "oversampling", &buffer.oversampling);
	createRegister8(SPI::SAMPLESTAKEN, "samplestaken", &buffer.samplestaken);

	par_enabled = createParameter("enabled");
}

SPI::~SPI()
{

}

void SPI::createRegister16(RegisterID rid, const std::string& id, uint16_t* p)
{
	Parameter* par = createParameter(id);
	reg16bit[par] = p;
	id2par[rid]= par;
}

void SPI::createRegister8(RegisterID rid, const std::string& id, uint8_t* p)
{
	Parameter* par = createParameter(id);
	reg8bit[par] = p;
	id2par[rid]= par;
}

Parameter* SPI::getRegister(RegisterID rid)
{
	if (id2par.find(rid) != id2par.end())
		return id2par[rid];
	std::clog << "Warning: unable to find SPI register: " << rid << std::endl;
	return 0;
}

void SPI::calculateBefore()
{
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
			HAL::getInstance()->wiringPiSPIDataRW(0, (unsigned char*)&buffer, sizeof(buffer));

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

void SPI::copyToParameters()
{
	for (auto iter = reg16bit.begin(); iter!= reg16bit.end(); iter++) {
		Parameter* p = iter->first;
		uint16_t* dest = iter->second;
		*p = (double)*dest;
	}

	for (auto iter = reg8bit.begin(); iter!= reg8bit.end(); iter++) {
		Parameter* p = iter->first;
		uint8_t* dest = iter->second;
		*p = (double)*dest;
	}
}

void SPI::copyFromParameters()
{
//	for (int i=0; i<nrBytes; i++) {
//		printf ("0x%02x ", byteArray[i]);
//	}
//	printf("\n");
	for (auto iter = reg16bit.begin(); iter!= reg16bit.end(); iter++) {
		Parameter* p = iter->first;
		uint16_t* dest = iter->second;
		*dest = (uint16_t) *p;
	}

	for (auto iter = reg8bit.begin(); iter!= reg8bit.end(); iter++) {
		Parameter* p = iter->first;
		uint8_t* dest = iter->second;
		*dest = (uint8_t) *p;
	}

}


bool SPI::isEnabled()
{
	if (enabled && *par_enabled==0.0) {
		// SPI got disabled
		Mbed2Pi->setEnabled(false);
		Pi2Mbed->setEnabled(false);
		enabled = false;
		return enabled;
	}

	if (!enabled && *par_enabled != 0.0) {
		// SPI got enabled
		Mbed2Pi->setEnabled(true);
		Pi2Mbed->setEnabled(true);
		enabled = true;
		return enabled;
	}

	// In case of no change
	return enabled;
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

void SPI::calculateAfter() {
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
			HAL::getInstance()->wiringPiSPIDataRW(0, (unsigned char*) &buffer, sizeof(buffer));
	} catch (int to) {
		std::cerr << "Timeout on SPI bus during write, reset" << std::endl;
		Pi2Mbed->set(1.0);
	}
}
