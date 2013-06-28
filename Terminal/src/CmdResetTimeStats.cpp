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

void CmdResetTimeStats::displayHelp() {
	std::cout << "Usage: " << getName() << std::endl;
	std::cout << "\tReset all timing statistics." << std::endl;

}
void CmdResetTimeStats::execute(std::list<std::string>& /*args*/) {
	Parameter* reset;

	reset = new Parameter(TimeStats::par_reset);

	reset->set(1.0);
	std::cout << "OK" << std::endl;
}
