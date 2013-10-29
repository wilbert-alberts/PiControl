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
		par_nrIncrements(createParameter("Dev.nrIncrements",4096.0, nrIncrements)),

		par_voltage(createParameter("Dev.voltage",0.0, voltage)),
		par_voltageGain(createParameter("Dev.voltageGain",1.0, voltageGain)),
		par_voltageOffset(createParameter("Dev.voltageOffset",0.0, voltageOffset)),

		par_dutycycle(createParameter("Dev.dutycycle",0.0, dutycycle))
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

	return p->get();
}

void Devices::setDevice(DeviceID id, double v) {
	if (devices.find(id) == devices.end())
		throw std::invalid_argument("Devices::getDevice: illegal value for deviceID: " + id);

	Parameter* p = devices[id];

	return p->set(v);
}

void Devices::sampleAngle(double frequency) {
	// Start with shifting previous values
	angle_[2] = angle_[1];
	angle_[1] = angle_[0];
	angle_v[1] = angle_v[0];
	// determine raw angle
	double hpos = spi->getRegister(SPI::HEIGHT1);
	double npos = spi->getRegister(SPI::HEIGHT2);
	double raw_angle = hpos - npos;
	par_rawAngle->set(raw_angle);
	// calculate new angle with gain and offset
	angle_[0] = raw_angle * par_angleGain->get() + par_angleOffset->get();
	angle_v[0] = (angle_[0] - angle_[1]) * frequency;
	angle_v[1] = (angle_[1] - angle_[2]) * frequency;
	angle_a[0] = (angle_v[0] - angle_v[1]) * frequency;
	// store calculated values into parameters
	par_angle->set(angle_[0]);
	par_angleA->set(angle_a[0]);
}

void Devices::sample() {
	double frequency = pt->getFrequency();

	sampleAngle(frequency);
	samplePosition(frequency);
	sampleBattery();
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
	par_rawPos->set(rawpos);

	int delta = rawpos - prevRawPos;
	// store new value for prevRawPos
	prevRawPos = rawpos;
	int nrIncs = par_nrIncrements->get();

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
	pos_[0] = encPos * par_posGain->get() + par_posOffset->get();

	// Calculate velocity and acceleration
	pos_v[0] = (pos_[0] - pos_[1]) * frequency;
	pos_v[1] = (pos_[1] - pos_[2]) * frequency;
	pos_a[0] = (pos_v[0] - pos_v[1]) * frequency;

	// store calculated values into parameters
	par_pos->set(pos_[0]);
	par_posV->set(pos_v[0]);
	par_posA->set(pos_a[0]);
}

void Devices::sampleBattery()
{
	par_voltage->set(spi->getRegister(SPI::UBAT)*par_voltageGain->get()+par_voltageOffset->get());
}

void Devices::update(void* /*context*/) {
	Devices* d = Devices::getInstance();

	d->update();
}

void Devices::update() {
	updateDC();
}

void Devices::updateDC() {
	double dc = par_dutycycle->get();
	// Limit dc to -1.0 -  1.0
	dc = dc> 1.0?  1.0 : dc;
	dc = dc<-1.0? -1.0 : dc;

	// Determine value for direction register
	int dir = dc<0.0 ? 1 : 0;
	int rawDC = (int)(65535.0*fabs(dc));

	spi->setRegister(SPI::PWM, static_cast<double>(rawDC));
	spi->setRegister(SPI::MOTORDIR, static_cast<double>(dir));
}
