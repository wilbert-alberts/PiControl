/*
 * TimeStats.cpp
 *
 *  Created on: Jun 16, 2013
 *      Author: wilbert
 */

#include <iostream>

#include "TimeStats.h"

#include "Parameter.h"
#include "DoubleBuffer.h"
#include "PeriodicTimer.h"

TimeStats::TimeStats(PeriodicTimer* _pt, DoubleBuffer* db)
: pt(_pt)
{
	// TODO Auto-generated constructor stub
	par = new Parameter(db, "sampleDuration");
}

TimeStats::~TimeStats() {
	// TODO Auto-generated destructor stub
}

void TimeStats::tick(void* context)
{
	TimeStats* my = static_cast<TimeStats*>(context);

	my->par->set(my->pt->getPeriod()-my->pt->getMargin());

//	std::cout << "." << std::endl;
}

