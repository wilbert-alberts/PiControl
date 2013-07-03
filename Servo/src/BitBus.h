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

class BitBus {
public:
	BitBus(unsigned char* bytes, int nrBytes);
	virtual ~BitBus();

	void createRegister(int id, const std::string& name, int startBit,
			int length);

	void setRegister(int id, int value);
	int  getRegister(int id);

private:
	void clearBit(int bit);
	void setBit(int bit);
	void setBit(int bit, int value);

	int nrBytes;
	unsigned char* bytes;

	class Register {
	public:
		int startBit;
		int length;
	};

	std::map<int, Register> registers;
};

#endif /* BITBUS_H_ */
