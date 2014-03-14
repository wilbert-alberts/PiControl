/*
 * CmdResetTimeStats.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdResetTimeStats.h"
#include "TimeStats.h"
#include "Parameter.h"

#include <iostream>
#include <string>
#include <list>

CmdResetTimeStats::CmdResetTimeStats() :
		Command("resetTiming") {
}

CmdResetTimeStats::~CmdResetTimeStats() {}

void CmdResetTimeStats::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << std::endl;
	output << "\tReset all timing statistics." << std::endl;

}
void CmdResetTimeStats::execute(std::ostream& /* output*/) {
	Parameter* __attribute__ ((unused)) reset(new Parameter(TimeStats::par_reset,1.0));
}
