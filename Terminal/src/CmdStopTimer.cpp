/*
 * StopTimer.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: wilbert
 */

#include "CmdStopTimer.h"

#include "DoubleBuffer.h"
#include "Parameter.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

Cmd_StopTimer::Cmd_StopTimer() :
		Command("stopTimer") {
}

Cmd_StopTimer::~Cmd_StopTimer() {
}

void Cmd_StopTimer::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << std::endl;
	output << "\tStops execution of Servo loop." << std::endl;
}

void Cmd_StopTimer::execute(std::ostream& /*output*/) {
	DoubleBufferLock dbl;
	Parameter* __attribute__ ((unused)) par = new Parameter("PeriodicTimer.stopRunning",1.0);
}
