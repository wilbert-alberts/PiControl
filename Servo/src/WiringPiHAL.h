/*
 * HAL.h
 *
 *  Created on: Jul 2, 2013
 *      Author: wilbert
 */

#ifndef WIRINGPIHAL_H_
#define WIRINGPIHAL_H_

#ifdef REALMODE

#include "HAL.h"

class WiringPiHAL : public HAL {
public:
	static void registerHAL();

	virtual void setup();
	virtual void pinMode(int pin, int mode);
	virtual int  digitalRead(int pin);
	virtual void digitalWrite(int pin, int value);
	virtual void wiringPiSPIDataRW(int channel, unsigned char *data, int len);
};

#endif

#endif /* HAL_H_ */

