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

	minMargin = new Parameter(TimeStats::par_minMargin);
	maxMargin = new Parameter(TimeStats::par_maxMargin);
	margin = new Parameter(TimeStats::par_margin);

	std::cout << "margin:    " << margin->get() << std::endl;
	std::cout << "minMargin: " << minMargin->get() << std::endl;
	std::cout << "maxMargin: " << maxMargin->get() << std::endl;

}

