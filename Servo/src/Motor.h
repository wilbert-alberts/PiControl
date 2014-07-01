/*
 * Motor.h
 *
 *  Created on: Jul 9, 2013
 *      Author: wilbert
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "ServoModule.h"

class Parameter;
class DigitalOut;
class Devices;

#include <random>

class Motor: public ServoModule {
public:
	Motor(ServoModule* predecessor);

	virtual ~Motor();

	void setTorque(double torque);
	void setDC(double dc);
	void calculateAfter();

	void setDevices(Devices* devs) {
		this->devs = devs;
	}

private:

	Devices* devs;

	Parameter* enabled;
	Parameter* nrIncrements;

	Parameter* t;
	Parameter* dutycycle;
	Parameter* ki;
	Parameter* kv;
	Parameter* rm;

	Parameter*  rotVelo;

	Parameter*  batVoltage;
	Parameter*  motorCurrent;

};

#endif /* MOTOR_H_ */
