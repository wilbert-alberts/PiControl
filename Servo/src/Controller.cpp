/*
 * Controller.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: wilbert
 */

#include "Controller.h"
#include "Motor.h"
#include "Devices.h"
#include "Parameter.h"


Controller* Controller::instance = 0;

Controller* Controller::getInstance() {
	if (instance == 0)
		instance = new Controller();
	return instance;
}

Controller::Controller() {
	enabled = new Parameter("Controller.enabled", 0);

	motor = Motor::getInstance();
	devs = Devices::getInstance();
	updateActualPosition = true;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::sample() {
	double tq;
	if (updateActualPosition) {
		// Get actual position from Devices
	}

	// Determine controller error


	// Calculate controller


	// Set torque.
	if (enabled->get() != 0.0) {
		motor->setTorque(tq);

	} else {
		motor->setTorque(0.0);
		updateActualPosition = true;
	}
}

void Controller::sample(void* context) {
	Controller* me = static_cast<Controller*>(context);
	me->sample();
}
