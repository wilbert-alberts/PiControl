/*
 * TimeStats.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#include "DoubleBuffer.h"
#include "Parameter.h"
#include "TimeStats.h"
#include "TimeStats_Term.h"

#include <iostream>
#include <string>
#include <cstring>

const std::string TimeStats_Term::dumpTimingCommand="dumpTiming";
const std::string TimeStats_Term::resetTimingCommand="resetTiming";


TimeStats_Term::TimeStats_Term() {}

TimeStats_Term::~TimeStats_Term() {}

void TimeStats_Term::execDumpTiming(int argc, char** /*argv[]*/)
{
  if (argc==1) {
    dumpTiming();
  }
  else {
    std::clog << "usage: resetTiming" << std::endl;
  }
}

void TimeStats_Term::execResetTiming(int argc, char** /*argv[]*/)
{
  if (argc==1) {
    resetTiming();
  }
  else {
    std::clog << "usage: resetTiming" << std::endl;
  }
}

void TimeStats_Term::dumpTiming() {
  Parameter* minMargin;
  Parameter* maxMargin;
  Parameter* margin;

  minMargin = new Parameter(par_minMargin);
  maxMargin = new Parameter(par_maxMargin);
  margin = new Parameter(par_margin);

  std::cout << "margin:    " << margin->get() <<  std::endl;
  std::cout << "minMargin: " << minMargin->get() <<  std::endl;
  std::cout << "maxMargin: " << maxMargin->get() <<  std::endl;
}

void TimeStats_Term::resetTiming() {
  Parameter* reset;

  reset = new Parameter(par_reset);

  reset->set(1.0);
  std::cout<<"OK"<< std::endl;
}
