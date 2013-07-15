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

Cmd_UpdateFrequency::Cmd_UpdateFrequency() {
	par_frequency = new Parameter(TimeStats::par_frequency,
			PeriodicTimer::getInstance()->getFrequency());
}

Cmd_UpdateFrequency::~Cmd_UpdateFrequency() {
}

void Cmd_UpdateFrequency::execute()
{
	double f = par_frequency->get();
	//std::clog << "frequency: " << f << std::endl;
	if (f!=freq) {
		PeriodicTimer::getInstance()->updateFrequency(f);
		freq = f;
	}
}

void Cmd_UpdateFrequency::execute(void*context)
{
	Cmd_UpdateFrequency* c = Cmd_UpdateFrequency::getInstance();
	c->execute();
}
