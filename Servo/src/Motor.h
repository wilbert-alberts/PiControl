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

class Motor {
public:
	static Motor* getInstance();
	virtual ~Motor();

	static void sample(void* context);
	void sample();

private:
	Motor();
	void updateInputs();
	void calculateModel();
	void setOutputs();

	Parameter* enabled;
	Parameter* t;
	Parameter* ki;
	Parameter* kv;
	Parameter* rm;
	Parameter* velo;

	// Raw Output
	Parameter*  pwmOut;
	Parameter*  motorDir;

	// Raw Input
	Parameter*  batVoltage;
	Parameter*  motorCurrent;
	Parameter*  encPos;
	Parameter*  height1;
	Parameter*  height2;

	// Deduced sensors
	Parameter*  angle;
	Parameter*  prevAngle;
	Parameter*  motorPos;


	static Motor* instance;
};

#endif /* MOTOR_H_ */
