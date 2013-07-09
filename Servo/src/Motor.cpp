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
	enabled = new Parameter("Motor.enabled");
	enabled->setDeep(0.0);

	t = new Parameter("Motor.torque");
	ki = new Parameter("Motor.ki");
	kv = new Parameter("Motor.kv");
	rm = new Parameter("Motor.rm");
	velo = new Parameter("Motor.velo");

	motorDir = new Parameter("Motor.direction");
	batVoltage = new Parameter("Motor.batVoltage");
	motorCurrent = new Parameter("Motor.current");
	motorPos = new Parameter("Motor.pos");
	pwmOut = new Parameter("Motor.pwmOut");

	angle = new Parameter("Motor.Angle");
	prevAngle = new Parameter("Motor.prevAngle");

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
