/*
 * DigitalIn.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "DigitalIn.h"
#include "Parameter.h"

#ifdef WIRINGPI
#include "wiringPi.h"
#endif

std::map<const std::string, DigitalIn*> DigitalIn::instances;


DigitalIn::DigitalIn(const std::string& name, int p) :
		pin(p), par(new Parameter(name)) {

}

DigitalIn::~DigitalIn() {
}

DigitalIn* DigitalIn::create(const std::string& name, int pin) {
	DigitalIn* r = new DigitalIn(name, pin);
	DigitalIn::instances[name] = r;
	return r;
}

int DigitalIn::get() {
	int v;
#ifdef WIRINGPI
	pinMode(pin, INPUT);
	v = digitalRead(pin)
	par->set(v);
#endif
	return v;
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
