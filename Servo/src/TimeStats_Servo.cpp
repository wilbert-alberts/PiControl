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
#include "TimeStats_Servo.h"
#include "PeriodicTimer.h"

Parameter* TimeStats_Servo::minMargin = 0;
Parameter* TimeStats_Servo::maxMargin = 0;
Parameter* TimeStats_Servo::margin = 0;
Parameter* TimeStats_Servo::reset = 0;

TimeStats_Servo::TimeStats_Servo() {
	// TODO Auto-generated constructor stub

}

TimeStats_Servo::~TimeStats_Servo() {
	// TODO Auto-generated destructor stub
}

void TimeStats_Servo::initSample(DoubleBuffer* db, PeriodicTimer* pt)
{
	minMargin = new Parameter(db, par_minMargin);
	maxMargin = new Parameter(db, par_maxMargin);
	margin = new Parameter(db, par_margin);
	reset = new Parameter(db, par_reset);

	pt->resetStats();
}

void TimeStats_Servo::sampleCommand(void* context)
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
