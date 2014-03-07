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

Motor::Motor()
: ServoModule("Motor", 0)
, ndis(0.0, 1.0)
{
	enabled = createParameter("enabled");

	t = createParameter("torque");
	ki = createParameter("ki");
	kv = createParameter("kv");
	rm = createParameter("rm");
	rotVelo = createParameter("rotationalVelo");
	dutycycle = createParameter("dutycycle");
	batVoltage = createParameter("batVoltage");
	motorCurrent = createParameter("current");

	devs = Devices::getInstance();
}

Motor::~Motor() {
}

void Motor::setTorque(double tq) {
	*t = tq;
}

void Motor::sample() {
	// Calculate rotational velocity
	double rvel = devs->getDeviceValue(Devices::posV);
	double nrIncs = devs->getDeviceValue(Devices::nrIncrements);

	*batVoltage= devs->getDeviceValue(Devices::voltage);

	rvel = 2*M_PI*rvel/nrIncs; // Note rotations per second!
	*rotVelo = rvel;

	double im =  *t / *ki;             // Motor current.
	*motorCurrent = im;
	double vme = im * *rm + rvel * *kv;
	double dc = vme / *batVoltage;

	// Normalize dc
	if (dc > 1)  dc = 1.0;
	if (dc < -1) dc = -1.0;

	dutycycle->set(dc);
	if (*enabled != 0.0) {
		devs->setDevice(Devices::dutycycle, dc);
	}
	else {
		//devs->setDevice(Devices::dutycycle, 0.0);
	}
}

