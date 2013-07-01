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

BitBus::BitBus(int n)
: nrBits(n)
{
	bytes = new char[nrBits/8+1];
	shadowBytes = new char[nrBits/8+1];

	for (int i=0; i<=nrBits/8; i++) {
		bytes[i] = 0;
	}
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

void BitBus::copyBytesTo(char* dst)
{
	memcpy (dst, bytes, nrBits/8+1);
}

void BitBus::copyBytesFrom(char* src)
{
	memcpy (bytes, src, nrBits/8+1);
}
