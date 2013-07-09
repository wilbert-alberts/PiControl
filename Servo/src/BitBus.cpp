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

BitBus::BitBus()
{
}

BitBus::~BitBus() {
}

void BitBus::createRegister(int id, const std::string& name, int startBit, int length)
{
	if (registers.find(id) != registers.end())
	{
		std::string n = "register id " + id;
		n += " ("+ name +") already present in BitBus";
		throw std::invalid_argument(n);
	}
	Register r;
	r.startBit = startBit;
	r.length = length;

	registers[id] = r;
}

void BitBus::setRegister(unsigned char* bytes, int id, int value) {
	if (registers.find(id) == registers.end())
		throw std::invalid_argument("unknown register id in setRegister");

	const Register& r = registers[id];

	if (value > (1<<r.length)) {
		std::clog << "Warning: value " << value << " does not fit in register " << id << std::endl;
		std::clog << "Skipping assignment." << std::endl;
		return;
	}

	for (int i=0; i<r.length;i++) {
		setBit(bytes, i+r.startBit, value&1);
		value = value>>1;
	}
}

int BitBus::getRegister(unsigned char* bytes, int id) {
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

void BitBus::clearBit(unsigned char* bytes, int bit)
{
	int byteNr = bit/8;
	char mask = 1<< (bit % 8);

	bytes[byteNr] = bytes[byteNr] & (~mask);
}

void BitBus::setBit(unsigned char* bytes, int bit)
{
	int byteNr = bit/8;
	char mask = 1 << (bit % 8);

	bytes[byteNr] = bytes[byteNr] | (mask);
}


void BitBus::setBit(unsigned char* bytes, int bit, int value)
{
	int byteNr = bit/8;
	char mask = 1 << (bit % 8);
	char vmask = (value==0) ? 0 : 1<< (bit%8);

	bytes[byteNr] = (bytes[byteNr] & (~mask)) | vmask;
}
