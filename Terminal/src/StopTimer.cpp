/*
 * StopTimer.cpp
 *
 *  Created on: Jun 22, 2013
 *      Author: wilbert
 */

#include "DoubleBuffer.h"
#include "Parameter.h"
#include "StopTimer.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

const std::string StopTimer::stopTimerCommand("stopTimer");

StopTimer::StopTimer() {}

StopTimer::~StopTimer() {}

void StopTimer::execStopTimer(int argc, char** /*argv[]*/)
{
  if (argc!=1)
    throw std::runtime_error("Usage " +  stopTimerCommand);

  // TODO: replace hardcoded parameter id by reference.
  Parameter* par = new Parameter("PeriodicTimer.stopRunning");
  par->set(1.0);
}
