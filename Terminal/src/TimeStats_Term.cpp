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
#include "TimeStats_Term.h"

const std::string TimeStats_Term::dumpTimingCommand="dumpTiming";
const std::string TimeStats_Term::resetTimingCommand="resetTiming";


TimeStats_Term::TimeStats_Term() {
	// TODO Auto-generated constructor stub

}

TimeStats_Term::~TimeStats_Term() {
	// TODO Auto-generated destructor stub
}

void TimeStats_Term::execDumpTiming(DoubleBuffer* db, int argc, char* argv[])
{
	if (argc==1) {
		dumpTiming(db);
	}
	else {
		std::cerr << "Usage: resetTiming" << std::endl;
	}
}

void TimeStats_Term::execResetTiming(DoubleBuffer* db, int argc, char* argv[])
{
	if (argc==1) {
		resetTiming(db);
	}
	else {
		std::cerr << "Usage: resetTiming" << std::endl;
	}
}

void TimeStats_Term::dumpTiming(DoubleBuffer* db) {
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

void TimeStats_Term::resetTiming(DoubleBuffer* db) {
	Parameter* reset;
	reset = new Parameter(db, par_reset);

	reset->set(1.0);
	std::cout<<"OK"<< std::endl;
}

