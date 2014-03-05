/*
 * Devices.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: wilbert
 */

#include "Devices.h"

#include "SPI.h"
#include "Parameter.h"
#include "PeriodicTimer.h"

#include <stdexcept>
#include <iostream>
#include <cmath>

Devices* Devices::instance=0;

Devices* Devices::getInstance()
{
	if (instance == 0)
		instance = new Devices();
	return instance;
}

Devices::Devices() :
		spi(SPI::getInstance()), pt(PeriodicTimer::getInstance()),

		par_h1Ang(createParameter("Dev.h1Ang",0.0, h1Ang)),
		par_h2Ang(createParameter("Dev.h2Ang",0.0, h2Ang)),

		par_h1AngGain(new Parameter("Dev.h1Ang_gain",1.0)),
		par_h2AngGain(new Parameter("Dev.h2Ang_gain",1.0)),

		par_h1AngOffset(new Parameter("Dev.h1Ang_offset",0.0)),
		par_h2AngOffset(new Parameter("Dev.h2Ang_offset",0.0)),

		par_rawAngle(createParameter("Dev.rawAngle",0.0, rawAngle)),
		par_angle(createParameter("Dev.angle",0.0, angle)),
		par_angleV(createParameter("Dev.angleV",0.0, angleV)),
		par_angleA(createParameter("Dev.angleA",0.0, angleA)),
		par_angleGain(createParameter("Dev.angleGain",1.0, angleGain)),
		par_angleOffset(createParameter("Dev.angleOffset",0.0, angleOffset)),

		prevRawPos(0), encPos(0),

		par_rawPos(createParameter("Dev.rawPos",0.0, rawPos)),
		par_pos(createParameter("Dev.pos",0.0, pos)),
		par_posV(createParameter("Dev.posV",0.0, posV)),
		par_posA(createParameter("Dev.posA",0.0, posA)),
		par_posGain(createParameter("Dev.posGain",1.0, posGain)),
		par_posOffset(createParameter("Dev.posOffset",0.0, posOffset)),

		par_rawGyro(createParameter("Dev.rawGyro",0.0, rawGyro)),
		par_gyro(createParameter("Dev.gyro",0.0, gyro)),
		par_gyroGain(createParameter("Dev.gyroGain",0.0, gyroGain)),
		par_gyroOffset(createParameter("Dev.gyroOffset",0.0, gyroOffset)),

		par_nrIncrements(createParameter("Dev.nrIncrements",4096.0, nrIncrements)),

		par_voltage(createParameter("Dev.voltage",0.0, voltage)),
		par_voltageGain(createParameter("Dev.voltageGain",1.0, voltageGain)),
		par_voltageOffset(createParameter("Dev.voltageOffset",0.0, voltageOffset)),

		par_dutycycle(createParameter("Dev.dutycycle",0.0, dutycycle)),
		par_motordir(createParameter("Dev.motordir",1.0, motordir)),
		par_oversampling(createParameter("Dev.oversampling",25.0, oversampling))

{
}

Devices::~Devices() {
	// TODO Auto-generated destructor stub
}

Parameter* Devices::createParameter(const std::string& name, double v, Devices::DeviceID id)
{
	Parameter* result = new Parameter(name, v);

	devices[id]=result;
	return result;
}

double Devices::getDeviceValue(DeviceID id) {
	if (devices.find(id) == devices.end())
		throw std::invalid_argument("Devices::getDevice: illegal value for deviceID: " + id);

	Parameter* p = devices[id];

	return *p;
}

void Devices::setDevice(DeviceID id, double v) {
	if (devices.find(id) == devices.end())
		throw std::invalid_argument("Devices::getDevice: illegal value for deviceID: " + id);

	Parameter* p = devices[id];
	*p = v;
}

void Devices::sampleAngle(double frequency) {
	// Start with shifting previous values
	angle_[2] = angle_[1];
	angle_[1] = angle_[0];
	angle_v[1] = angle_v[0];
	// determine raw angle
	double h1 = spi->getRegister(SPI::HEIGHT1);
	*par_h1Ang = h1;
	double h2 = spi->getRegister(SPI::HEIGHT2);
	*par_h2Ang = h2;
	double raw_angle = (*par_h1AngGain*h1 + *par_h1AngOffset) -
			           (*par_h2AngGain*h2 + *par_h2AngOffset);
	*par_rawAngle = raw_angle;
	// calculate new angle with gain and offset
	angle_[0] = raw_angle * *par_angleGain + *par_angleOffset;
	angle_v[0] = (angle_[0] - angle_[1]) * frequency;
	angle_v[1] = (angle_[1] - angle_[2]) * frequency;
	angle_a[0] = (angle_v[0] - angle_v[1]) * frequency;
	// store calculated values into parameters
	*par_angle = angle_[0];
	*par_angleA = angle_a[0];
}

void Devices::sample() {
	double frequency = pt->getFrequency();

	sampleAngle(frequency);
	sampleGyro(frequency);
	samplePosition(frequency);
	sampleBattery();
}

void Devices::sampleGyro(double /*frequency*/)
{
	double rawGyro = spi->getRegister(SPI::GYRO);
	if (rawGyro > 32767)
		rawGyro -= 65536;
	double gyro = rawGyro * *par_gyroGain + *par_gyroOffset;
	*par_rawGyro = rawGyro;
	*par_gyro = gyro;

}

void Devices::sample(void* /*context*/) {
	Devices* d = Devices::getInstance();

	d->sample();
}

void Devices::samplePosition(double frequency) {
	// Start with shifting previous values
	pos_[2] = pos_[1];
	pos_[1] = pos_[0];
	pos_v[1] = pos_v[0];

	unsigned int rawpos = static_cast<unsigned int>(spi->getRegister(
			SPI::ENCPOS));
	// Mask relevant bits
	rawpos = rawpos & 0x0fff; // 12 bits
	// determine delta w.r.t. previous sample;
	*par_rawPos = rawpos;

	int delta = rawpos - prevRawPos;
	// store new value for prevRawPos
	prevRawPos = rawpos;
	int nrIncs = *par_nrIncrements;

	// Correct for over/underruns
	if (delta > nrIncs / 2) {
		delta -= nrIncs;
	} else {
		if (delta < -nrIncs / 2) {
			delta += nrIncs;
		}
	}

	// Update encoder position
	encPos += delta;

	// Transform into real position
	pos_[0] = encPos * *par_posGain + *par_posOffset;

	// Calculate velocity and acceleration
	pos_v[0] = (pos_[0] - pos_[1]) * frequency;
	pos_v[1] = (pos_[1] - pos_[2]) * frequency;
	pos_a[0] = (pos_v[0] - pos_v[1]) * frequency;

	// store calculated values into parameters
	*par_pos = pos_[0];
	*par_posV = pos_v[0];
	*par_posA = pos_a[0];
}

void Devices::sampleBattery()
{
	*par_voltage = spi->getRegister(SPI::UBAT) * *par_voltageGain+ *par_voltageOffset;
}

void Devices::update(void* /*context*/) {
	Devices* d = Devices::getInstance();

	d->update();
}

void Devices::update() {
	updateDC();
	spi->setRegister(SPI::OVERSAMPLING, *par_oversampling);
}

void Devices::updateDC() {
	double dc = *par_dutycycle*(*par_motordir < 0? -1: 1);
	// Limit dc to -1.0 -  1.0
	dc = dc> 1.0?  1.0 : dc;
	dc = dc<-1.0? -1.0 : dc;

	// Determine value for direction register
	int dir = dc<0.0 ? 1 : 0;
	int rawDC = (int)(65535.0*fabs(dc));

	spi->setRegister(SPI::PWM, static_cast<double>(rawDC));
	spi->setRegister(SPI::MOTORDIR, static_cast<double>(dir));
}
