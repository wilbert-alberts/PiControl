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

class Motor {
public:
	static Motor* getInstance();
	virtual ~Motor();

	void setTorque(double torque);
	static void sample(void* context);
	void sample();

private:
	Motor();
	void calculateModel();

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

	static Motor* instance;
};

#endif /* MOTOR_H_ */
