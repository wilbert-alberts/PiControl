/*
 * StopTimer.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: wilbert
 */

#include "DoubleBuffer.h"
#include "Parameter.h"
#include "StopTimer.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

StopTimer::StopTimer() :
		Command("stopTimer") {
}

StopTimer::~StopTimer() {
}

void StopTimer::displayHelp() {
	std::cout << "Usage: " << getName() << std::endl;
	std::cout << "\tStops execution of Servo loop." << std::endl;
}

void StopTimer::execute(std::list<std::string>& args) {
	if (!args.empty())
		throw std::runtime_error("Usage " + getName());

	Parameter* par = new Parameter("PeriodicTimer.stopRunning");
	par->set(1.0);
}
