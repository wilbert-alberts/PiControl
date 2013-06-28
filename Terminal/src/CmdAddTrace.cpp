/*
 * CmdAddTrace.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdAddTrace.h"

#include "Traces_Term.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>

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
	int length;

	if (args.empty()) {
		displayHelp();
		return;
	}

	std::string name = args.front();
	args.pop_front();

	if (!args.empty()) {
		std::stringstream ss(args.front());
		ss >> length;

	} else {
		length = Traces_Term::DefaultTraceLength;
	}

	if (!args.empty()) {
		displayHelp();
		return;
	}

	Traces_Term* traces = Traces_Term::getInstance();

	traces->createTrace(name, length);
}
