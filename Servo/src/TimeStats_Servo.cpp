/*
 * TimeStats.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#include "DoubleBuffer.h"
#include "Parameter.h"
#include "TimeStats_Servo.h"
#include "PeriodicTimer.h"

#include <iostream>
#include <string>

Parameter* TimeStats_Servo::sampleCounter = 0;
Parameter* TimeStats_Servo::minMargin = 0;
Parameter* TimeStats_Servo::maxMargin = 0;
Parameter* TimeStats_Servo::margin = 0;
Parameter* TimeStats_Servo::reset = 0;
Parameter* TimeStats_Servo::overruns = 0;

TimeStats_Servo::TimeStats_Servo() {}

TimeStats_Servo::~TimeStats_Servo() {}

void TimeStats_Servo::initSample()
{
  minMargin = new Parameter(par_minMargin,0.0);
  maxMargin = new Parameter(par_maxMargin,0.0);
  margin = new Parameter(par_margin,0.0);
  reset = new Parameter(par_reset,0.0);
  overruns = new Parameter(par_overruns,0.0);
  sampleCounter = new Parameter(par_sampleCounter,0.0);

  PeriodicTimer::getInstance()->resetStats();
}

void TimeStats_Servo::takeTimeStamp(void* context)
{
	unsigned int t = PeriodicTimer::getInstance()->getTimeElapsed();

	Parameter* p = static_cast<Parameter*>(context);

	*p = t;
}

void TimeStats_Servo::sampleCommand(void* /*context*/)
{
  PeriodicTimer* pt = PeriodicTimer::getInstance();

  std::cerr << "sc before: " <<  *sampleCounter << std::endl;
  *sampleCounter = *sampleCounter + 1.0;
  std::cerr << "sc after: " <<  *sampleCounter << std::endl;
  *margin = pt->getMargin();
  *minMargin = pt->getMinMargin();
  *maxMargin = pt->getMaxMargin();
  *overruns = pt->getNrOverruns();
  if (*reset > 0.0) {
    pt->resetStats();
    *reset = 0.0;
  }
}
