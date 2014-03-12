/*
 * DigitalIn.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "DigitalIn.h"
#include "Parameter.h"

#include "HAL.h"

std::map<const std::string, DigitalIn*> DigitalIn::instances;


DigitalIn::DigitalIn(const std::string& name, int p) :
		pin(p), par(new Parameter(name,0.0)), enabled(false) {

}

DigitalIn::~DigitalIn() {
}

DigitalIn* DigitalIn::create(const std::string& name, int pin) {
	DigitalIn* r = new DigitalIn(name, pin);
	DigitalIn::instances[name] = r;
	return r;
}

int DigitalIn::get() {
	int v=0;
	if (enabled)
		v = HAL::getInstance()->digitalRead(pin);
	par->set(v);
	return v;
}

void DigitalIn::setEnabled(bool v) {
	if (v and not enabled) {
		HAL::getInstance()->pinMode(pin, HAL::IN);
	}
	enabled = v;
	get();
}


DigitalIn* DigitalIn::getByName(const std::string& name) {
	if (instances.find(name) != instances.end()) {
		return instances[name];
	}
	return 0;
}

void DigitalIn::captureAllIns(void* /*context*/) {
	for (std::map<const std::string, DigitalIn*>::iterator iter = instances.begin();
			iter != instances.end();
			iter++) {
		DigitalIn* digo = iter->second;

		digo->get();
	}
}

DigitalIns::DigitalIns()
: ServoModule("DigitalIn", 0)
{

}

DigitalIns::~DigitalIns() {
}

DigitalIns* DigitalIns::getInstance()
{
	static DigitalIns* instance;

	if (instance==0) {
		instance = new DigitalIns();
	}
	return instance;
}

void DigitalIns::sample()
{
	DigitalIn::captureAllIns(0);
}
