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

CmdUpdateFrequency::CmdUpdateFrequency(ServoModule* predecessor)
: ServoModule("CmdUpdateFrequency", predecessor)
, freq(0.0)
{
	par_frequency = createParameter(TimeStats::par_frequency);
}

CmdUpdateFrequency::~CmdUpdateFrequency() {
}


void CmdUpdateFrequency::calculateAfter()
{
	double f = *par_frequency;
	if (f!=freq) {
		getPeriodicTimer()->updateFrequency(f);
		freq = f;
	}
}

