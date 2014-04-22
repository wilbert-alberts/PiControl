/*
 * TimeStats.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#include "CmdDumpTiming.h"

#include "Parameter.h"
#include "TimeStats.h"
#include "DoubleBuffer.h"

#include <iostream>
#include <string>
#include <list>

CmdDumpTiming::CmdDumpTiming() :
		Command("dumpTiming") {
}

CmdDumpTiming::~CmdDumpTiming() {
}

void CmdDumpTiming::displayHelp(std::ostream& out)
{
	out << "Usage: " << getName() << std::endl;
	out << "\tShow timing statistics." << std::endl;
}

void CmdDumpTiming::execute(std::ostream& output) {
	DoubleBufferLock dbl;
	Parameter* minMargin;
	Parameter* maxMargin;
	Parameter* margin;
	Parameter* frequency;

	minMargin = new Parameter(TimeStats::par_minMargin,0.0);
	maxMargin = new Parameter(TimeStats::par_maxMargin,0.0);
	margin = new Parameter(TimeStats::par_margin,0.0);
	frequency= new Parameter(TimeStats::par_frequency,0.0);

	output << "frequency: " << *frequency << "Hz (period: " << 1.0 / *frequency<< " seconds)" << std::endl;


	unsigned int periodInUs = 1000000 / *frequency;
	unsigned int actualPeriod = periodInUs - *margin;
	unsigned int smallestPeriod = periodInUs - *maxMargin;
	unsigned int largestPeriod = periodInUs - *minMargin;

	output << "actual period:   " << actualPeriod/1000.0 << "ms ("<< 100.0*actualPeriod/periodInUs << "%)" << std::endl;
	output << "largest period:  " << largestPeriod/1000.0 << "ms ("<< 100.0*largestPeriod/periodInUs << "%)" << std::endl;
	output << "smallest period: " << smallestPeriod/1000.0 << "ms ("<< 100.0*smallestPeriod/periodInUs << "%)" << std::endl;
}

