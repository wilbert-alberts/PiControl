/*
 * NewDevice.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: wilbert
 */

#include "NewDevice.h"

#include "SPI.h"
#include "Parameter.h"
#include "Filter.h"

#include <map>

NewDevices::NewDevices(ServoModule* other, SPI* spi)
: ServoModule("Dev", other)
{
	devices[ENCPOS] = new GODevice(new Encoder(spi));
	devices[H1] = new GODevice(new SPIDevice("h1", spi->getRegister(SPI::HEIGHT1)));
	devices[H2] = new GODevice(new SPIDevice("h2", spi->getRegister(SPI::HEIGHT2)));
	devices[HEIGHT] = new LPFDevice(new D2Ang(devices[H1], devices[H2]));
	devices[GYRO] = new LPFDevice(new GODevice(new SPIDevice("gyro", spi->getRegister(SPI::GYRO))));
}

NewDevices::~NewDevices()
{
}

void NewDevices::calculateBefore()
{
	for (auto iter = devices.begin(); iter!=devices.end(); iter++)
	{
		NDevice* d = iter->second;
		d->readDevice();
	}
}

void NewDevices::calculateAfter()
{
	for (auto iter = devices.begin(); iter!=devices.end(); iter++)
	{
		NDevice* d = iter->second;
		d->writeDevice();
	}
}

NDevice* NewDevices::getDevice(DeviceID did)
{
	if (devices.find(did) != devices.end())
		return devices[did];
	return 0;
}

NDevice::NDevice(const std::string& _id)
: id(_id)
, value(createParameter("value"))
{
}

Parameter* NDevice::createParameter(const std::string pid)
{
	return new Parameter(id + "." + pid);
}

Parameter* NDevice::createParameter(const std::string pid, double v)
{
	return new Parameter(id + "." + pid, v);
}

NDevice::operator double()
{
	return *value;
};

NDevice& NDevice::operator=(const double other)
{
	*value = other;
	return *this;
};


GODevice::GODevice(NDevice* raw)
: NDevice("go")
, rawDevice(raw)
, gain(createParameter("gain",1.0))
, offset(createParameter("offset",0.0))
{
}

void GODevice::readDevice()
{
	rawDevice->readDevice();
	*value = *rawDevice* (*gain) + (*offset);
}

LPFDevice::LPFDevice(NDevice* raw)
: NDevice(raw->getID() + ".lpf")
, rawDevice(raw)
, filter(new Filter(getID() + ".filter" ,3))
{
};

void LPFDevice::readDevice()
{
	rawDevice->readDevice();
	*value = filter->calculate(*rawDevice);
}

Encoder::Encoder(SPI* spi)
: NDevice("Encoder")
, spiPos(spi->getRegister(SPI::ENCPOS))
, prevSpiPos(createParameter("prevRawPos"))
, nrIncrements(createParameter("nrIncrements", 4000))
{
};

void Encoder::readDevice() {
	unsigned int rawpos = static_cast<unsigned int>(*spiPos);
	// Mask relevant bits
	rawpos = rawpos & 0x0fff; // 12 bits
	// determine delta w.r.t. previous sample;
	int delta = rawpos - *prevSpiPos;
	// store new value for prevRawPos
	*prevSpiPos = rawpos;
		// Correct for over/underruns
	if (delta > *nrIncrements/ 2) {
		delta -= *nrIncrements;
	} else {
		if (delta < -*nrIncrements / 2) {
			delta += *nrIncrements;
		}
	}
	// Update encoder position
	*value = *value + (double)delta;
}

SPIDevice::SPIDevice(const std::string& id, Parameter* spiRegister)
: NDevice(id)
, spiH(spiRegister)
{

}

void SPIDevice::readDevice()
{
	*value = *spiH;
}

D2Ang::D2Ang(NDevice* d1, NDevice* d2)
: NDevice("heightAngle")
, devH1(d1)
, devH2(d2)
{

}

void D2Ang::readDevice()
{
	*value = *devH1 - *devH1;
}
