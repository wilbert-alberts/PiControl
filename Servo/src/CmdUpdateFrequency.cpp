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

#include <cassert>

CmdUpdateFrequency::CmdUpdateFrequency(ServoModule* predecessor)
: ServoModule("CmdUpdateFrequency", predecessor)
, pt(0)
, freq(1)
{
	par_frequency = createParameter("frequency");
}

CmdUpdateFrequency::~CmdUpdateFrequency() {
}

void CmdUpdateFrequency::setPeriodicTimer(PeriodicTimer* _pt)
{
	pt = _pt;

	freq = pt->getFrequency();
	par_frequency->setDeep(freq);
}

void CmdUpdateFrequency::calculateAfter()
{
	assert(pt!=0);
	if (*par_frequency != freq) {
		freq = *par_frequency;
		getPeriodicTimer()->updateFrequency(freq);
	}
}
