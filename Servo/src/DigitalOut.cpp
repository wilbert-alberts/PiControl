/*
 * DigitalOut.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "DigitalOut.h"
#include "Parameter.h"

#include "HAL.h"

#include <iostream>

std::map<const std::string, DigitalOut*> DigitalOut::instances;

DigitalOut::DigitalOut(const std::string& name, int p, int v) :
		pin(p), par(new Parameter(name, v)) {
	HAL::getInstance()->pinMode(pin, HAL::OUT);
	HAL::getInstance()->digitalWrite(pin, v);
}

DigitalOut::~DigitalOut() {
	// TODO Auto-generated destructor stub
}

DigitalOut* DigitalOut::create(const std::string& name, int pin, int value) {
	DigitalOut* r = new DigitalOut(name, pin, value);
	DigitalOut::instances[name] = r;
	return r;
}

void DigitalOut::set(int v) {
	HAL::getInstance()->digitalWrite(pin, v);
	par->set(v != 0 ? 1.0 : 0.0);
}

int DigitalOut::get() {
	return (par->get() > 0.0) ? 1 : 0;
}

DigitalOut* DigitalOut::getByName(const std::string& name) {
	if (instances.find(name) != instances.end()) {
		return instances[name];
	}
	return 0;
}

void DigitalOut::activateAllOuts(void* /*context*/) {
	for (std::map<const std::string, DigitalOut*>::iterator iter =
			instances.begin(); iter != instances.end(); iter++) {
		DigitalOut* digo = iter->second;

		HAL::getInstance()->digitalWrite(digo->pin, digo->get());

	}
}
