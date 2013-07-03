/*
 * SPI.h
 *
 *  Created on: Jul 1, 2013
 *      Author: ben
 */

#ifndef SPI_H_
#define SPI_H_

class BitBus;
class DigitalOut;
class DigitalIn;

class SPI {
public:
	SPI();
	virtual ~SPI();

	void readBus();
	void writeBus();

	static void writeBus(void* context);
	static void readBus(void* context);

	BitBus* getBB();

private:
	void waitOnSignal(DigitalIn* in, double value, unsigned int timeoutInUs);

	int             nrBytes;
	unsigned char*  byteArray;
	BitBus*     bb;
	DigitalOut* Pi2Mbed;
	DigitalIn*  Mbed2Pi;

};

#endif /* SPI_H_ */
