/*
 * ServoModule.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#include "ServoModule.h"
#include "Parameter.h"
#include "DigitalIn.h"
#include "DigitalOut.h"
#include "PeriodicTimer.h"

#include <iostream>

ServoModule::ServoModule(const std::string& _id, ServoModule* _other)
: id(_id)
, other(_other)
, tsStart1(createParameter(id+"1.tsStart"))
, tsEnd1(createParameter(id+"1.tsEnd"))
, tsStart2(createParameter(id+"2.tsStart"))
, tsEnd2(createParameter(id+"2.tsEnd"))
, measureTiming(1)
{
}

ServoModule::~ServoModule() {
}

void ServoModule::sample(PeriodicTimer* pt) {
	this->pt = pt;

	if (measureTiming)
		*tsStart1 = pt->getTime();
	calculateBefore();
	*tsEnd1 = pt->getTime();

	if (other != 0)
		other->sample(pt);

	*tsStart2 = pt->getTime();
	calculateAfter();
	if (measureTiming)
		*tsEnd2 = pt->getTime();
}

void ServoModule::calculateBefore()
{
}

void ServoModule::calculateAfter()
{
}


Parameter* ServoModule::createParameter(const std::string& id) {
	return createParameter(id, 0.0);
}
	Parameter* ServoModule::createParameter(const std::string& id, const double value) {
	return new Parameter(getId() + "." + id, value);
}

DigitalIn* ServoModule::createDigitalIn(const std::string& id, int pin) {
	return DigitalIn::create(getId() + "." + id, pin);
}

DigitalOut* ServoModule::createDigitalOut(const std::string& id, int pin, int value) {
	return DigitalOut::create(getId() + "." + id, pin, value);
}
