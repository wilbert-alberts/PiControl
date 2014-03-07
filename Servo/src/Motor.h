/*
 * Motor.h
 *
 *  Created on: Jul 9, 2013
 *      Author: wilbert
 */

#ifndef MOTOR_H_
#define MOTOR_H_

class Parameter;
class DigitalOut;
class Devices;

#include "ServoModule.h"

#include <random>

class Motor : public ServoModule {
public:
	static Motor* getInstance();
	virtual ~Motor();

	virtual void sample();

	void setTorque(double torque);


private:
	Motor();
	double doInject(double dc);

	Devices* devs;

	Parameter* enabled;
	Parameter* t;
	Parameter* ki;
	Parameter* kv;
	Parameter* rm;

	Parameter*  rotVelo;
	Parameter*  dutycycle;

	Parameter*  batVoltage;
	Parameter*  motorCurrent;

	std::default_random_engine generator;
	std::normal_distribution<double> ndis;
	Parameter* injAmpl;
	Parameter* injFreq;
	Parameter* noiseSample;


	static Motor* instance;
};

#endif /* MOTOR_H_ */
