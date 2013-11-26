/*
 * CmdAddTrace.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdAddTrace.h"

#include "Traces.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <stdexcept>

CmdAddTrace::CmdAddTrace() :
		Command("addTrace") {
}

CmdAddTrace::~CmdAddTrace() {
}

void CmdAddTrace::displayHelp() {
	std::cout << "Usage: " << getName() << " <parameter> [ <length> ]"
			<< std::endl;
	std::cout << "\tStart tracing <parameter>." << std::endl;
}

void CmdAddTrace::execute(std::list<std::string>& args) {

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

	Traces* traces = Traces::getInstance();

	int idx = Parameter::findParameter(name);

	if (idx<1)
		throw std::runtime_error("Unknown parameter: " + name );

	traces->addTrace(new Parameter(idx));
}
