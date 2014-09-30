/*
 * Motor.cpp
 *
 *  Created on: Jul 9, 2013
 *      Author: wilbert
 */

#define Dont_ALTERNATING_PULSE_TO_DETECT_DIRECTION_DEPENDENT_EFFECT

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
}

void Motor::setDC(double dc) {
	*dutycycle = dc;
}

void Motor::calculateAfter() {
	assert(devs!=0);

#ifdef MOTORMODEL_NEEDED
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
#endif

#ifdef ALTERNATING_PULSE_TO_DETECT_DIRECTION_DEPENDENT_EFFECT
	static int counter(0);
	static int inc(1);
	static int upper(1);
	static int nrperiods(0);

	// If end of positive period is reached, start negative period.
	if (counter >=upper) {
		inc = -1;
	}
	// If end of negative period is reached, start new positive period
	if (counter < 0) {
		inc = 1;
		// Count the number of periods.
		nrperiods++;

		// Check whether period length should be changed
		if (nrperiods>25/upper) {
			// Reset period counter
			nrperiods=0;

			// Increase period length
			upper+=1;

			// Check whether max period length has been achieved.
			// If so, start with minimal period length.
			if (upper == 30)
				upper = 1;
		}
	}
	counter += inc;
	((double)inc)/7.5;
	*dutycycle = ((double)inc)/7.5;
#endif

	// Normalize dutycycle
	if (*dutycycle > 1)  *dutycycle = 1.0;
	if (*dutycycle < -1) *dutycycle = -1.0;

	*devs->getDevice(Devices::DC) = *dutycycle;
}
