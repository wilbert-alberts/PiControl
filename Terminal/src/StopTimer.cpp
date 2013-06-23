/*
 * StopTimer.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: wilbert
 */

#include <iostream>
#include <string>
#include <cstdlib>

#include "DoubleBuffer.h"
#include "Parameter.h"

#include "StopTimer.h"



const std::string StopTimer::stopTimerCommand("stopTimer");


StopTimer::StopTimer() {
	// TODO Auto-generated constructor stub

}

StopTimer::~StopTimer() {
	// TODO Auto-generated destructor stub
}

void StopTimer::execStopTimer(int argc, char* argv[])
{
	if (argc!=1) {
		std::cerr << "Usage: "<< stopTimerCommand << std::endl;
		exit(-1);
	}

	// TODO: replace hardcoded parameter id by reference.
	Parameter* par = new Parameter("PeriodicTimer.stopRunning");
	par->set(1.0);
}

