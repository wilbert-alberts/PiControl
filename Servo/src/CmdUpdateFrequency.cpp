/*
 * CmdUpdateFrequency.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: wilbert
 */

#include "CmdUpdateFrequency.h"

#include "Parameter.h"
#include "PeriodicTimer.h"
#include "TimeStats.h"

#include <iostream>

Cmd_UpdateFrequency* Cmd_UpdateFrequency::instance = 0;

Cmd_UpdateFrequency* Cmd_UpdateFrequency::getInstance()
{
	if (instance == 0)
		instance = new Cmd_UpdateFrequency();
	return instance;
}

Cmd_UpdateFrequency::Cmd_UpdateFrequency()
: ServoModule("Cmd_UpdateFrequency",100)
{

	par_frequency = createParameter(TimeStats::par_frequency,
			PeriodicTimer::getInstance()->getFrequency());
}

Cmd_UpdateFrequency::~Cmd_UpdateFrequency() {
}

void Cmd_UpdateFrequency::sample()
{
	double f = *par_frequency;
	//std::clog << "frequency: " << f << std::endl;
	if (f!=PeriodicTimer::getInstance()->getFrequency()) {
		PeriodicTimer::getInstance()->updateFrequency(f);
	}
}
