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

const std::string TimeStats::dumpTimingName="dumpTiming";
const std::string TimeStats::resetTimingName="resetTiming";

const std::string par_minMargin("minMargin");
const std::string par_maxMargin("maxMargin");
const std::string par_margin("margin");
const std::string par_reset("reset");


TimeStats::TimeStats() {
	// TODO Auto-generated constructor stub

}

TimeStats::~TimeStats() {
	// TODO Auto-generated destructor stub
}

void TimeStats::execDumpTimingCommand(DoubleBuffer* db, int argc, char* argv[])
{
	if (argc==1) {
		dumpTiming(db);
	}
	else {
		std::cerr << "Usage: resetTiming" << std::endl;
	}
}

void TimeStats::execResetTimingCommand(DoubleBuffer* db, int argc, char* argv[])
{
	if (argc==1) {
		resetTiming(db);
	}
	else {
		std::cerr << "Usage: resetTiming" << std::endl;
	}
}

void TimeStats::dumpTiming(DoubleBuffer* db) {
	Parameter* minMargin;
	Parameter* maxMargin;
	Parameter* margin;

	minMargin = new Parameter(db, par_minMargin);
	maxMargin = new Parameter(db, par_maxMargin);
	margin = new Parameter(db, par_margin);

	std::cout << "margin:    " << margin->get() <<  std::endl;
	std::cout << "minMargin: " << minMargin->get() <<  std::endl;
	std::cout << "maxMargin: " << maxMargin->get() <<  std::endl;
}

void TimeStats::resetTiming(DoubleBuffer* db) {
	Parameter* reset;
	reset = new Parameter(db, par_reset);

	reset->set(1.0);
	std::cout<<"OK"<< std::endl;
}

