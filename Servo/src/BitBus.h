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
	BitBus();
	virtual ~BitBus();

	void createRegister(int id, const std::string& name, int startBit,
			int length);

	void setRegister(unsigned char* bytes, int id, int value);
	int  getRegister(unsigned char* bytes, int id);

private:
	void clearBit(unsigned char* bytes, int bit);
	void setBit(unsigned char* bytes, int bit);
	void setBit(unsigned char* bytes, int bit, int value);

	class Register {
	public:
		int startBit;
		int length;
	};

	std::map<int, Register> registers;
};

#endif /* BITBUS_H_ */
