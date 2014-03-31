/*
 * NewDevice.h
 *
 *  Created on: Mar 31, 2014
 *      Author: wilbert
 */

#ifndef NEWDEVICE_H_
#define NEWDEVICE_H_

#include "ServoModule.h"

#include <map>
#include <string>

class Parameter;
class SPI;
class Filter;

class NDevice {
public:
	NDevice(const std::string& _id);
	virtual ~NDevice() {};

	Parameter* createParameter(const std::string pid);
	Parameter* createParameter(const std::string pid, double v);

	const std::string& getID() {return id; };

	virtual operator double();
	virtual NDevice& operator=(const double other);

	virtual void readDevice() {};
	virtual void writeDevice() {};

private:
	const std::string id;

protected:
	Parameter* value;

};

class GODevice : public  NDevice {
public:
	GODevice(NDevice* raw);
	virtual ~GODevice() {};
	virtual void readDevice();
	virtual void writeDevice() {};

private:
	NDevice*   rawDevice;
	Parameter* gain;
	Parameter* offset;
};

class LPFDevice : public NDevice {
public:
	LPFDevice(NDevice* raw);
	~LPFDevice() {};

	virtual void readDevice();
private:
	NDevice* rawDevice;
	Filter* filter;
};

class Encoder : public NDevice
{
public:
	Encoder(SPI* spi);
	void readDevice();

private:
	Parameter* spiPos;
	Parameter* prevSpiPos;
	Parameter* nrIncrements;
};

class SPIDevice: public NDevice
{
public:
	SPIDevice(const std::string& id, Parameter* spiRegister);
	void readDevice();

private:
	Parameter* spiH;
};

class D2Ang: public NDevice
{
public:
	D2Ang(NDevice* d1, NDevice* d2);
	void readDevice();

private:
	NDevice* devH1;
	NDevice* devH2;
};



class NewDevices: public ServoModule {
public:
	typedef enum
	{
		ENCPOS,
		H1,
		H2,
		HEIGHT,
		GYRO
	} DeviceID;

	NewDevices(ServoModule* other, SPI* spi);
	virtual ~NewDevices();

	void calculateBefore();
	void calculateAfter();
	NDevice* getDevice(DeviceID did);

private:
	std::map<DeviceID, NDevice*> devices;
};

#endif /* NEWDEVICE_H_ */


