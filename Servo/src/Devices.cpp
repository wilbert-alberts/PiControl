/*
 * NewDevice.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: wilbert
 */

#include "Devices.h"

#include "SPI.h"
#include "Parameter.h"
#include "Filter.h"

#include <map>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <functional>

Devices::Devices(ServoModule* other)
: ServoModule("Dev", other)
{
	devices[ENCPOS] = new GODevice(new Encoder(SPI::ENCPOS));
	devices[ENCPOS_D] = new DDevice(devices[ENCPOS]);
	devices[H1] = new GODevice(new SPIDevice("Dev.h1", SPI::HEIGHT1));
	devices[H2] = new GODevice(new SPIDevice("Dev.h2", SPI::HEIGHT2));
	devices[HEIGHT] = new LPFDevice(new D2Ang(devices[H1], devices[H2]));
	devices[GYRO] = new LPFDevice(new GODevice(new SPIDevice("Dev.gyro", SPI::GYRO)));
	devices[UBAT] = new SPIDevice("Dev.ubat", SPI::UBAT);
	devices[DC] = new DutyCycle(SPI::PWM, SPI::MOTORDIR);

	readDevices.insert(devices[ENCPOS_D]);
	readDevices.insert(devices[HEIGHT]);
	readDevices.insert(devices[GYRO]);
	readDevices.insert(devices[UBAT]);

	writeDevices.insert(devices[DC]);
}

Devices::~Devices()
{
}

void Devices::setSPI(SPI* spi)
{
	for (auto iter = devices.begin(); iter!=devices.end(); iter++)
	{
		Device* d = iter->second;
		d->setSPI(spi);
	}
}

void Devices::calculateBefore()
{
	std::for_each(readDevices.begin(), readDevices.end(), std::mem_fun(&Device::readDevice));
}

void Devices::calculateAfter()
{
	std::for_each(writeDevices.begin(), writeDevices.end(), std::mem_fun(&Device::writeDevice));
}

Device* Devices::getDevice(DeviceID did)
{
	if (devices.find(did) != devices.end())
		return devices[did];
	return 0;
}

Device::Device(const std::string& _id)
: id(_id)
, value(createParameter("value"))
{
}

Parameter* Device::createParameter(const std::string pid)
{
	return new Parameter(id + "." + pid);
}

Parameter* Device::createParameter(const std::string pid, double v)
{
	return new Parameter(id + "." + pid, v);
}

Device::operator double()
{
	return *value;
};

Device& Device::operator=(const double other)
{
	*value = other;
	return *this;
};

GODevice::GODevice(Device* raw)
: Device(raw->getID() + std::string(".go"))
, rawDevice(raw)
, gain(createParameter("gain",1.0))
, offset(createParameter("offset",0.0))
{
}

void GODevice::setSPI(SPI* spi)
{
	rawDevice->setSPI(spi);
}

void GODevice::readDevice()
{
	rawDevice->readDevice();
	*value = *rawDevice* (*gain) + (*offset);
}



LPFDevice::LPFDevice(Device* raw)
: Device(raw->getID() + ".lpf")
, rawDevice(raw)
, filter(new Filter(getID() + ".filter" ,3))
{
};

void LPFDevice::setSPI(SPI* spi)
{
	rawDevice->setSPI(spi);
}

void LPFDevice::readDevice()
{
	rawDevice->readDevice();
	*value = filter->calculate(*rawDevice);
}

Encoder::Encoder(SPI::RegisterID _spiReg)
: Device("Dev.Encoder")
, spiReg(_spiReg)
, spiPos(0)
, prevSpiPos(createParameter("prevRawPos"))
, nrIncrements(createParameter("nrIncrements", 4000))
{
};

void Encoder::setSPI(SPI* spi)
{
	spiPos = spi->getRegister(spiReg);
}

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

SPIDevice::SPIDevice(const std::string& id, SPI::RegisterID  spiRegister)
: Device(id)
, spiReg(spiRegister)
, spiH(0)
{

}

void SPIDevice::setSPI(SPI* spi)
{
	spiH = spi->getRegister(spiReg);
}

void SPIDevice::readDevice()
{
	*value = *spiH;
}

D2Ang::D2Ang(Device* d1, Device* d2)
: Device("Dev.heightAngle")
, devH1(d1)
, devH2(d2)
{
}

void D2Ang::setSPI(SPI* spi)
{
	devH1->setSPI(spi);
	devH2->setSPI(spi);
}

void D2Ang::readDevice()
{
	devH1->readDevice();
	devH2->readDevice();

	*value = *devH1 - *devH1;
}

DDevice::DDevice(Device* _d)
: Device(_d->getID()+std::string("_D"))
, d(_d)
{
}

void DDevice::setSPI(SPI* spi)
{
	d->setSPI(spi);
}

void DDevice::readDevice()
{
	double oldValue(*d);

	d->readDevice();
	*value = *d - oldValue;
}

DutyCycle::DutyCycle(SPI::RegisterID _spiPw, SPI::RegisterID _spiDir)
: Device("Dev.DutyCycle")
, spiPw(_spiPw)
, spiDir(_spiDir)
, pw(0)
, dir(0)
{
}

void DutyCycle::setSPI(SPI* spi)
{
	pw = spi->getRegister(spiPw);
	dir = spi->getRegister(spiDir);
}

void DutyCycle::writeDevice()
{
	double dc;
	double val(*value);
	dc = (val) > 1.0 ?  1.0 : val;
	dc = (val) <-1.0 ? -1.0 : val;

	*pw = fabs(dc)*65535.0;
	*dir = (*value) <0 ? 1 : 0;
}
