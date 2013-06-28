/*
 * BitBus.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef BITBUS_H_
#define BITBUS_H_

#include <string>
#include <map>


class DigitalOut;
class DigitalIn;
class Parameter;


class BitBus {
public:
	BitBus(int spiChannel, int reqPin, int ackPin, int nrBits);
	virtual ~BitBus();

	void createRegister(int id, const std::string& name, int startBit,
			int length);

	void setRegister(int id, int value);
	int  getRegister(int id);

	void transmitBus();
	void receiveBus();

private:
	void clearBit(int bit);
	void setBit(int bit);
	void setBit(int bit, int value);

	int nrBits;
	char* bytes;
	char* shadowBytes;

	int spiChannel;
	DigitalOut* req;
	DigitalIn*  ack;


	class Register {
	public:
		int startBit;
		int length;
		Parameter* par;
	};

	std::map<int, Register> registers;
};

#endif /* BITBUS_H_ */
