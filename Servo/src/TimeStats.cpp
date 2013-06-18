/*
 * TimeStats.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#include <iostream>
#include <string>
#include <cstring>

#include "Parameter.h"
#include "TimeStats.h"
#include "PeriodicTimer.h"

const std::string TimeStats::par_minMargin("minMargin");
const std::string TimeStats::par_maxMargin("maxMargin");
const std::string TimeStats::par_margin("margin");
const std::string TimeStats::par_reset("reset");


Parameter* TimeStats::minMargin = 0;
Parameter* TimeStats::maxMargin = 0;
Parameter* TimeStats::margin = 0;
Parameter* TimeStats::reset = 0;

TimeStats::TimeStats() {
	// TODO Auto-generated constructor stub

}

TimeStats::~TimeStats() {
	// TODO Auto-generated destructor stub
}

void TimeStats::initSample(DoubleBuffer* db, PeriodicTimer* pt)
{
	minMargin = new Parameter(db, par_minMargin);
	maxMargin = new Parameter(db, par_maxMargin);
	margin = new Parameter(db, par_margin);
	reset = new Parameter(db, par_reset);

	pt->resetStats();
}

void TimeStats::sampleCommand(void* context)
{
	PeriodicTimer* pt = static_cast<PeriodicTimer*>(context);

	margin->set(pt->getMargin());
	minMargin->set(pt->getMinMargin());
	maxMargin->set(pt->getMaxMargin());

	if (reset->get() > 0.0) {
		pt->resetStats();
		reset->set(0.0);
	}
}
