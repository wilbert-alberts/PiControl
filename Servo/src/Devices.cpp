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

Devices::Devices() :
		spi(SPI::getInstance()), pt(PeriodicTimer::getInstance()),

		par_rawAngle(new Parameter("Dev.rawAngle",0.0)),
		par_angle(new Parameter("Dev.angle",0.0)),
		par_angleV(new Parameter("Dev.angleV",0.0)),
		par_angleA(new Parameter("Dev.angleA",0.0)),
		par_angleGain(new Parameter("Dev.angleGain",1.0)),
		par_angleOffset(new Parameter("Dev.angleOffset",0.0)),

		par_rawPos(new Parameter("Dev.rawAngle",0.0)),
		par_pos(new Parameter("Dev.pos",0.0)),
		par_posV(new Parameter("Dev.posV",0.0)),
		par_posA(new Parameter("Dev.posA",0.0)),
		par_posGain(new Parameter("Dev.posGain",1.0)),
		par_posOffset(new Parameter("Dev.posOffset",0.0)) {
}

Devices::~Devices() {
	// TODO Auto-generated destructor stub
}

void Devices::sampleAngle(double frequency) {
	// Start with shifting previous values
	angle[2] = angle[1];
	angle[1] = angle[0];
	angle_v[1] = angle_v[0];
	// determine raw angle
	double hpos = spi->getRegister(SPI::HEIGHT1);
	double npos = spi->getRegister(SPI::HEIGHT2);
	double raw_angle = hpos - npos;
	par_rawAngle->set(raw_angle);
	// calculate new angle with gain and offset
	angle[0] = raw_angle * par_angleGain->get() + par_angleOffset->get();
	angle_v[0] = (angle[0] - angle[1]) * frequency;
	angle_v[1] = (angle[1] - angle[2]) * frequency;
	angle_a[0] = (angle_v[0] - angle_v[1]) * frequency;
	// store calculated values into parameters
	par_angle->set(angle[0]);
	par_angleV->set(angle_v[0]);
	par_angleA->set(angle_a[0]);
}

void Devices::sample() {
	double frequency = pt->getFrequency();

	sampleAngle(frequency);
	samplePosition(frequency);
}

void Devices::samplePosition(double frequency) {
	// Start with shifting previous values
	pos[2] = pos[1];
	pos[1] = pos[0];
	pos_v[1] = pos_v[0];

	unsigned int rawpos = static_cast<unsigned int>(spi->getRegister(
			SPI::ENCPOS));
	// Mask relevant bits
	rawpos = rawpos & 0x0fff; // 12 bits
	// determine delta w.r.t. previous sample;
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
	pos[0] = encPos * par_posGain->get() + par_posOffset->get();

	// Calculate velocity and acceleration
	pos_v[0] = (pos[0] - pos[1]) * frequency;
	pos_v[1] = (pos[1] - pos[2]) * frequency;
	pos_a[0] = (pos_v[0] - pos_v[1]) * frequency;
}
