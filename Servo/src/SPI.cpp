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


#include <string>

SPI::SPI() {
	bb = new BitBus(60);

	bb->createRegister(1, std::string("Height1"), 16,0);
	bb->createRegister(2, std::string("Height2"), 16,16);
	bb->createRegister(3, std::string("UBat"), 16,32);
	bb->createRegister(4, std::string("EncPos"), 16,48);

	bb->createRegister(11, std::string("PWM"), 16, 0);
	bb->createRegister(12, std::string("MotorDir"), 8, 16);



	//bb.createRegister();
	//bb.createRegister();
	//bb.createRegister();


	byteArray = new char[1+60/8];
	Pi2Mbed = DigitalOut::create(std::string("pi2mbed"), 16, 1);
	Mbed2Pi = DigitalIn::create("mbed2pi", 18);

#ifdef WIRINGPI
	wiringPiSetup(0,1000000); //1 Mhz, channel doesn't matter
#endif


}

SPI::~SPI() {
	delete[]  byteArray;
}

void SPI::writeBus()
{
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	bb->copyBytesTo(byteArray);

	Pi2Mbed->set(1);
	while (Mbed2Pi->get() != 1.0);

#ifdef WIRINGPI
	wiringPiSPIDataRW(0, bytearray, 8);
#endif



}

void SPI::readBus()
{
	// Protocol to read from the bus consists of:
	// 1) Pulling Pi2Mbed to zero
	// 2) Wait until Mbed2Pi is zero
	// 3) Transmit over spi

	Pi2Mbed->set(0);
	while (Mbed2Pi->get() != 0.0);

#ifdef WIRINGPI
	wiringPiSPIDataRW(0, bytearray, 8);
#endif

	bb->copyBytesFrom(byteArray);
}


void SPI::writeBus(void* context)
{
	static_cast<SPI*>(context)->writeBus();
}

void SPI::readBus(void* context)
{
	static_cast<SPI*>(context)->readBus();
}

BitBus* SPI::getBB()
{
	return bb;
}

