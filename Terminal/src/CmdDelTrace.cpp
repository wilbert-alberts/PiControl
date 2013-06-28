/*
 * CmdDelTrace.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdDelTrace.h"

#include "Traces_Term.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>

CmdDelTrace::CmdDelTrace() :
		Command("delTrace") {
}

CmdDelTrace::~CmdDelTrace() {
}

void CmdDelTrace::displayHelp() {
	std::cout << "Usage: " << getName() << " <parameter>" << std::endl;
	std::cout << "\tStop tracing <parameter>." << std::endl;

}
void CmdDelTrace::execute(std::list<std::string>& args) {
	if (args.empty()) {
		displayHelp();
		return;
	}

	std::string name = args.front();
	args.pop_front();

	if (!args.empty()) {
		displayHelp();
		return;
	}

	Traces_Term* traces = Traces_Term::getInstance();

	traces->destroyTrace(name);
}
