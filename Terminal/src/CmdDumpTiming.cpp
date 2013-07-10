/*
 * TimeStats.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#include "Parameter.h"
#include "TimeStats.h"
#include "CmdDumpTiming.h"

#include <iostream>
#include <string>
#include <list>

CmdDumpTiming::CmdDumpTiming() :
		Command("dumpTiming") {
}

CmdDumpTiming::~CmdDumpTiming() {
}

void CmdDumpTiming::displayHelp()
{
	std::cout << "Usage: " << getName() << std::endl;
	std::cout << "\tShow timing statistics." << std::endl;
}

void CmdDumpTiming::execute(std::list<std::string>& args) {
	if (!args.empty()) {
		displayHelp();
	}

	Parameter* minMargin;
	Parameter* maxMargin;
	Parameter* margin;
	Parameter* frequency;

	minMargin = new Parameter(TimeStats::par_minMargin);
	maxMargin = new Parameter(TimeStats::par_maxMargin);
	margin = new Parameter(TimeStats::par_margin);
	frequency= new Parameter(TimeStats::par_frequency);

	std::cout << "frequency: " << frequency->get() << "Hz (period: " << 1.0/frequency->get()<< " seconds)" << std::endl;


	unsigned int periodInUs = 1000000/frequency->get();
	unsigned int actualPeriod = periodInUs - margin->get();
	unsigned int smallestPeriod = periodInUs - maxMargin->get();
	unsigned int largestPeriod = periodInUs - minMargin->get();


	std::cout << "actual period:   " << actualPeriod/1000.0 << "ms ("<< 100.0*actualPeriod/periodInUs << "%)" << std::endl;
	std::cout << "largest period:  " << largestPeriod/1000.0 << "ms ("<< 100.0*largestPeriod/periodInUs << "%)" << std::endl;
	std::cout << "smallest period: " << smallestPeriod/1000.0 << "ms ("<< 100.0*smallestPeriod/periodInUs << "%)" << std::endl;
}

