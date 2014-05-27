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

#include <assert.h>
#include <iostream>
#include <cmath>


Motor::Motor(ServoModule* sm)
: ServoModule("Motor", sm)
, devs(0)
{
	enabled = createParameter("enabled",0);
	nrIncrements = createParameter("nrIncrements",4000);

	t = createParameter("torque",0.0);
	ki = createParameter("ki",0.0);
	kv = createParameter("kv",0.0);
	rm = createParameter("rm",0.0);
	rotVelo = createParameter("rotationalVelo",0.0);
	dutycycle = createParameter("dutycycle",0.0);
	batVoltage = createParameter("batVoltage",0.0);
	motorCurrent = createParameter("current",0.0);
}

Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

void Motor::setTorque(double tq) {
	*t = tq;
	//t->set(tq);
}

void Motor::calculateAfter() {
	assert(devs!=0);

	// Calculate rotational velocity
	double rvel = *devs->getDevice(Devices::ENCPOS_D);

	*batVoltage= *devs->getDevice(Devices::UBAT);

	rvel = 2*M_PI*rvel/ (*nrIncrements); // Note rotations per second!
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
		*devs->getDevice(Devices::DC) = dc;
	}
	else {
		//devs->setDevice(Devices::dutycycle, 0.0);
	}
}
