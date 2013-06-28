/*
 * BitBus.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "BitBus.h"

#include "DigitalOut.h"
#include "DigitalIn.h"
#include "Parameter.h"


#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstring>

BitBus::BitBus(int spichannel, int reqPin, int ackPin, int n)
: nrBits(n), spiChannel(spichannel)
{
	bytes = new char[nrBits/8+1];
	shadowBytes = new char[nrBits/8+1];

	for (int i=0; i<=nrBits/8; i++) {
		bytes[i] = 0;
	}

	req = DigitalOut::create(std::string("BitBus.req"), reqPin, 0);
	ack = DigitalIn::create(std::string("BitBus.ack"),ackPin);
}

BitBus::~BitBus() {
	delete[] bytes;
}

void BitBus::createRegister(int id, const std::string& name, int startBit, int length)
{
	if (startBit + length > nrBits)
		throw std::range_error("startBit + length exceeds size of BitBus");

	if (registers.find(id) != registers.end())
		throw std::invalid_argument("register id already present in BitBus");

	Register r;
	r.startBit = startBit;
	r.length = length;
	r.par = new Parameter(name);

	registers[id] = r;
}

void BitBus::setRegister(int id, int value) {
	if (registers.find(id) == registers.end())
		throw std::invalid_argument("unknown register id in setRegister");

	const Register& r = registers[id];

	if (value > (1<<r.length)) {
		std::ostringstream ss;
		ss << "value " << value << " does not fit in register " << r.par->getName();
		throw std::invalid_argument(ss.str());
	}

	r.par->set(value);

	for (int i=0; i<r.length;i++) {
		setBit(i+r.startBit, value&1);
		value = value>>1;
	}
}

int BitBus::getRegister(int id) {
	if (registers.find(id) == registers.end())
		throw std::invalid_argument("unknown register id in setRegister");

	const Register& r = registers[id];

	int result=0;
	char* sresult = (char*)(&result);

	for (int i=0; i<r.length/8+1; i++) {
		sresult[i] = bytes[i+ r.startBit/8];
	}
	result = result >> r.startBit%8;

	return result;
}

void BitBus::clearBit(int bit)
{
	if (bit >= nrBits)
		throw std::range_error("bit exceeds size of BitBus in clearBit");

	int byteNr = bit/8;
	char mask = 1<< (bit % 8);

	bytes[byteNr] = bytes[byteNr] & (~mask);
}

void BitBus::setBit(int bit)
{
	if (bit >= nrBits)
		throw std::range_error("bit exceeds size of BitBus in setBit");

	int byteNr = bit/8;
	char mask = 1 << (bit % 8);

	bytes[byteNr] = bytes[byteNr] | (mask);
}


void BitBus::setBit(int bit, int value)
{
	if (bit >= nrBits)
		throw std::range_error("bit exceeds size of BitBus in setBit");

	int byteNr = bit/8;
	char mask = 1 << (bit % 8);
	char vmask = (value==0) ? 0 : 1<< (bit%8);

	bytes[byteNr] = (bytes[byteNr] & (~mask)) | vmask;
}

void BitBus::transmitBus()
{
	memcpy (shadowBytes, bytes, nrBits/8+1);
	req->set(0);
	while (ack->get() != 0.0)  { std::cout << "tb.";  std::cout.flush(); }

#ifdef WIRINGPI
	wiringPiSPIDataRW (int channel, shadowBytes, nrBits/8+1);
#endif
}

void BitBus::receiveBus()
{
	req->set(1);
	while (ack->get() == 0.0)  { std::cout << "rb.";  std::cout.flush(); };

#ifdef WIRINGPI
	wiringPiSPIDataRW (int channel, shadowBytes, nrBits/8+1);
#endif
	memcpy (bytes, shadowBytes, nrBits/8+1);
}

void BitBus::readBitBus(void* context)
{
	BitBus* bb = static_cast<BitBus*> (context);
	bb->receiveBus();
}
void BitBus::writeBitBus(void* context)
{
	BitBus* bb = static_cast<BitBus*> (context);
	bb->transmitBus();
}
