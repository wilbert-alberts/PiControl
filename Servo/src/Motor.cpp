/*
 * Motor.cpp
 *
 *  Created on: Jul 9, 2013
 *      Author: wilbert
 */

#include "Motor.h"
#include "Parameter.h"
#include "SPI.h"

#include <iostream>

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
	velo = new Parameter("Motor.velo",0.0);

	motorDir = new Parameter("Motor.direction",0.0);
	batVoltage = new Parameter("Motor.batVoltage",0.0);
	motorCurrent = new Parameter("Motor.current",0.0);
	motorPos = new Parameter("Motor.pos",0.0);
	pwmOut = new Parameter("Motor.pwmOut",0.0);

	angle = new Parameter("Motor.Angle",0.0);
	prevAngle = new Parameter("Motor.prevAngle",0.0);

}

Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

void Motor::sample() {
	updateInputs();
	calculateModel();
	std::cout << " Motor.enabled: " << enabled->get() << std::endl;
	if (enabled->get() != 0.0) {
		setOutputs();
	}
}

void Motor::updateInputs() {
	SPI* spi = SPI::getInstance();
	batVoltage->set(spi->getRegister(SPI::UBAT));

}

void Motor::calculateModel() {

}

void Motor::setOutputs() {

}

void Motor::sample(void* context) {
	Motor* me = static_cast<Motor*>(context);
	me->sample();
}
