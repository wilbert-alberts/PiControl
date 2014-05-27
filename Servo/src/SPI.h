/*
 * SPI.h
 *
 *  Created on: Jul 1, 2013
 *      Author: ben
 */

#ifndef SPI_H_
#define SPI_H_

#include "ServoModule.h"

#include <string>
#include <map>



class DigitalOut;
class DigitalIn;
class Parameter;

class SPI: public ServoModule {
public:
	SPI(ServoModule* wrapped);
	virtual ~SPI();

	void calculateBefore();
	void calculateAfter();

	typedef enum {
		HEIGHT1,
		HEIGHT2,
		UBAT,
		GYRO,
		GYROREF,
		ENCPOS,
		PWM,
		MOTORDIR,
		OVERSAMPLING,
		SAMPLESTAKEN,
		ACC
	} RegisterID;

	Parameter* getRegister(RegisterID  id);

private:
	class SPIRegisters
	{
	public:
		uint16_t height1;
		uint16_t height2;
		uint16_t ubat;
		uint16_t gyro;
		uint16_t gyroref;
		uint16_t encpos;
		uint16_t pwm;
		uint16_t motordir;
		uint16_t oversampling;
		uint16_t samplestaken;
		uint16_t acc;
	};

	SPIRegisters buffer;

	std::map<Parameter*, uint16_t*>  reg16bit;
	std::map<RegisterID, Parameter*> id2par;

	DigitalOut* Pi2Mbed;
	DigitalIn*  Mbed2Pi;
	Parameter* par_enabled;
	bool enabled;

	void createRegister16(RegisterID rid, const std::string& id, uint16_t* p);

	void waitOnSignal(DigitalIn* in, double value, unsigned int timeoutInUs);
	void copyFromParameters();
	void copyToParameters();
	bool isEnabled();
};


#endif /* SPI_H_ */
