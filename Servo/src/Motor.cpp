/*
 * Motor.cpp
 *
 *  Created on: Jul 9, 2013
 *      Author: wilbert
 */

#include "Motor.h"
#include "Parameter.h"
#include "SPI.h"
#include "Devices.h"

#include <iostream>
#include <cmath>

Motor* Motor::instance = 0;

Motor* Motor::getInstance() {
	if (instance == 0)
		instance = new Motor();
	return instance;
}

Motor::Motor() {
	enabled = new Parameter("Motor.enabled",0);

	t = new Parameter("Motor.torque",0.0);
	ki = new Parameter("Motor.ki",0.0);
	kv = new Parameter("Motor.kv",0.0);
	rm = new Parameter("Motor.rm",0.0);
	rotVelo = new Parameter("Motor.rotationalVelo",0.0);
	dutycycle = new Parameter("Motor.dutycycle",0.0);
	batVoltage = new Parameter("Motor.batVoltage",0.0);
	motorCurrent = new Parameter("Motor.current",0.0);

	devs = Devices::getInstance();
}

Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

void Motor::setTorque(double tq) {
	t->set(tq);
}

void Motor::sample() {
	// Calculate rotational velocity
	double rvel = devs->getDevice(Devices::posV);
	double nrIncs = devs->getDevice(Devices::nrIncrements);

	rvel = 2*M_PI*rvel/nrIncs; // Note rotations per second!
	rotVelo->set(rvel);

	double im =  t->get() / ki->get();             // Motor current.
	motorCurrent->set(im);
	double vme = im * rm->get() + rvel * kv->get();
	double dc = vme / batVoltage->get();

	// Normalize dc
	if (dc > 1)  dc = 1.0;
	if (dc < -1) dc = -1.0;

	dutycycle->set(dc);
	if (enabled->get() != 0.0) {
		devs->setDevice(Devices::dutycycle, dc);
	}
	else {
		devs->setDevice(Devices::dutycycle, 0.0);
	}
}

void Motor::sample(void* context) {
	Motor* me = static_cast<Motor*>(context);
	me->sample();
}
