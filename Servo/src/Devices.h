/*
 * NewDevice.h
 *
 *  Created on: Mar 31, 2014
 *      Author: wilbert
 */

#ifndef DEVICES_H
#define DEVICES_H

#include "ServoModule.h"
#include "SPI.h"

#include <map>
#include <set>
#include <string>

class Parameter;
class Filter;

class Device {
public:
	Device(const std::string& _id);
	virtual ~Device() {};
	virtual void setSPI(SPI*) {};

	Parameter* createParameter(const std::string pid);
	Parameter* createParameter(const std::string pid, double v);

	const std::string& getID() {return id; };

	virtual operator double();
	virtual Device& operator=(const double other);

	virtual void readDevice(int) {};
	virtual void writeDevice(int) {};

	void read(int f);
	void write(int f);


private:
	const std::string id;

	int fresh;

protected:
	Parameter* value;

};

class GODevice : public  Device {
public:
	GODevice(Device* raw);
	virtual ~GODevice() {};
	virtual void setSPI(SPI* spi);

	virtual void readDevice(int);
	virtual void writeDevice(int) {};

private:
	Device*   rawDevice;
	Parameter* gain;
	Parameter* offset;
};

class LPFDevice : public Device {
public:
	LPFDevice(Device* raw);
	virtual ~LPFDevice() {};
	virtual void setSPI(SPI* spi);

	virtual void readDevice(int f);
private:
	Device* rawDevice;
	Filter* filter;
};


class HPFDevice : public Device {
public:
	HPFDevice(Device* raw);
	virtual ~HPFDevice() {};
	virtual void setSPI(SPI* spi);

	virtual void readDevice(int f);
private:
	Device* rawDevice;
	Filter* filter;
};
class Encoder : public Device
{
public:
	Encoder(SPI::RegisterID spiReg);
	void readDevice(int f);
	void setSPI(SPI* spi);

private:
	SPI::RegisterID spiReg;
	Parameter* spiPos;
	Parameter* prevSpiPos;
	Parameter* nrIncrements;
};

class SPIDevice: public Device
{
public:
	SPIDevice(const std::string& id, SPI::RegisterID spiReg);
	void readDevice(int f);
	void setSPI(SPI* spi);

private:
	SPI::RegisterID spiReg;
	Parameter* spiH;
};

class DutyCycle: public Device
{
public:
	DutyCycle(SPI::RegisterID spiPw, SPI::RegisterID spiDir);
	void setSPI(SPI* spi);
	void writeDevice(int f);

private:
	SPI::RegisterID spiPw;
	SPI::RegisterID spiDir;
	Parameter* pw;
	Parameter* dir;
};



class D2Ang: public Device
{
public:
	D2Ang(Device* d1, Device* d2);
	virtual void readDevice(int f);
	virtual void setSPI(SPI* spi);

private:
	Device* devH1;
	Device* devH2;
};

class DDevice : public Device
{
public:
	DDevice(Device* d);
	virtual void readDevice(int f);
	virtual void setSPI(SPI* spi);

private:
	Device* d;
	double  prev;
};

class Counter: public Device
{
public:
	Counter();
	virtual void readDevice(int f);
private:
	double ctr;
};

class IDevice: public Device
{
public:
	IDevice(Device* d);
	virtual void readDevice(int f);
	virtual void setSPI(SPI* spi);

private:
	Device* d;
	double  i;
};

class Devices: public ServoModule {
public:
	typedef enum
	{
		ENCPOS,
		ENCPOS_D,
		H1,
		H2,
		HEIGHT,
		GYRO,
		ACC,
		NRINCREMENTS,
		UBAT,
		DC,
		TEST
	} DeviceID;

	Devices(ServoModule* other);
	virtual ~Devices();

	void setSPI(SPI* spi);

	void calculateBefore();
	void calculateAfter();
	Device* getDevice(DeviceID did);

private:
	std::map<DeviceID, Device*> devices;
	std::set<Device*>  readDevices;
	std::set<Device*>  writeDevices;

	int ctr;
};

#endif /* NEWDEVICE_H_ */


