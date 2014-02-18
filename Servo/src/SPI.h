/*
 * SPI.h
 *
 *  Created on: Jul 1, 2013
 *      Author: ben
 */

#ifndef SPI_H_
#define SPI_H_

#include <string>
#include <map>

class BitBus;
class DigitalOut;
class DigitalIn;
class Parameter;

class SPI {
public:
	static SPI* getInstance();
	virtual ~SPI();

	void readBus();
	void writeBus();

	static void writeBus(void* context);
	static void readBus(void* context);

	double getRegister(int reg);
	void   setRegister(int reg, double value);

	static constexpr int HEIGHT1 = 1;
	static constexpr int HEIGHT2 = 2;
	static constexpr int GYRO = 3;
	static constexpr int UBAT = 4;
	static constexpr int ENCPOS = 5;
	static constexpr int SAMPLESTAKEN = 6;

	static constexpr int PWM = 11;
	static constexpr int MOTORDIR = 12;
	static constexpr int OVERSAMPLING = 13;


private:
	static SPI* instance;
	SPI();

	void waitOnSignal(DigitalIn* in, double value, unsigned int timeoutInUs);
	void createRegister(int id, const std::string& n, int start, int length);
	void copyFromParameters();
	void copyToParameters();
	bool isEnabled();

	bool            bEnabled;
	Parameter*      enabled;
	int             nrBytes;
	unsigned char*  byteArray;
	BitBus*     bb;
	DigitalOut* Pi2Mbed;
	DigitalIn*  Mbed2Pi;

	std::map<int, Parameter*> registers;

};

#endif /* SPI_H_ */
