/*
 * ServoModule.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: wilbert
 */

#include "ServoModule.h"

#include "PeriodicTimer.h"
#include "Parameter.h"

#include <functional>


static void ServoModuleSample(void* context);

ServoModule::ServoModule()
: id("no-id")
{
	// TODO Auto-generated constructor stub

}

ServoModule::ServoModule(const std::string& _id, int calculationPrio)
: id(_id)
{
	PeriodicTimer* tmr;
	tmr = PeriodicTimer::getInstance();

	tmr->addPeriodicFunction(ServoModuleSample, this);
}

void ServoModuleSample(void* context)
{
	ServoModule* sm = static_cast<ServoModule*>(context);
	sm->sample();
}

ServoModule::~ServoModule() {
	// TODO Auto-generated destructor stub
}

Parameter* ServoModule::createParameter(const std::string& parId) {
	return createParameter(parId, 0.0);
}

Parameter* ServoModule::createParameter(const std::string& parId, const double d) {
	return new Parameter(id+"."+parId, d);
}

void ServoModule::sample() {
}
