/*
 * HAL.h
 *
 *  Created on: Jul 2, 2013
 *      Author: wilbert
 */

#ifndef HAL_H_
#define HAL_H_

class HAL {
public:
	HAL();
	virtual ~HAL();

	virtual void setup()=0;
	virtual void pinMode(int pin, int mode)=0;
	virtual int  digitalRead(int pin)=0;
	virtual void digitalWrite(int pin, int value)=0;
	virtual void wiringPiSPIDataRW(int channel, unsigned char *data, int len)=0;

	static HAL* getInstance();

	static const int IN=1;
	static const int OUT=2;

protected:
	static void registerHAL(HAL* i);

private:
	static HAL* instance;
};
#endif /* HAL_H_ */
