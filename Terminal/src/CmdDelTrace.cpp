/*
 * CmdDelTrace.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdDelTrace.h"

#include "Traces.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <stdexcept>

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

	Traces* traces = Traces::getInstance();

	int idx = Parameter::findParameter(name);

	if (idx<1)
		throw std::runtime_error("Unknown parameter: " + name );

	traces->delTrace(new Parameter(idx));
}

CmdDelAllTraces::CmdDelAllTraces() :
		Command("delAllTraces") {
}

CmdDelAllTraces::~CmdDelAllTraces() {
}

void CmdDelAllTraces::displayHelp() {
	std::cout << "Usage: " << getName() << std::endl;
	std::cout << "\tDelete all traces." << std::endl;

}
void CmdDelAllTraces::execute(std::list<std::string>& args) {
	if (!args.empty()) {
		displayHelp();
		return;
	}

	Traces* traces = Traces::getInstance();

	traces->delAllTraces();
}
