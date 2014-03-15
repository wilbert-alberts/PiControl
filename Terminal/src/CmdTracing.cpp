/*
 * CmdDelTrace.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdTracing.h"

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

void CmdAddTrace::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << " <parameter> [ <length> ]"
			<< std::endl;
	output << "\tStart tracing <parameter>." << std::endl;
}

void CmdAddTrace::execute(std::ostream& /*output*/) {
	std::string name = getNextArgumentAsString();

	Traces* traces = Traces::getInstance();

	int idx = Parameter::findParameter(name);

	if (idx<1)
		throw std::runtime_error("Unknown parameter: " + name );

	traces->addTrace(new Parameter(idx));
}

CmdDelTrace::CmdDelTrace() :
		Command("delTrace") {
}

CmdDelTrace::~CmdDelTrace() {
}

void CmdDelTrace::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << " <parameter>" << std::endl;
	output << "\tStop tracing <parameter>." << std::endl;

}
void CmdDelTrace::execute(std::ostream& /*output*/) {
	std::string name = getNextArgumentAsString();

	Traces* traces = Traces::getInstance();

	int idx = Parameter::findParameter(name);

	if (idx<1)
		throw std::runtime_error("Unknown parameter: " + name );

	traces->delTrace(new Parameter(idx));
}

CmdClearTrace::CmdClearTrace() :
		Command("clearTrace") {
}

CmdClearTrace::~CmdClearTrace() {
}

void CmdClearTrace::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << " <parameter>" << std::endl;
	output << "\tClear trace data for <parameter>." << std::endl;

}
void CmdClearTrace::execute(std::ostream& /*output*/) {
	std::string name = getNextArgumentAsString();
	Traces* traces = Traces::getInstance();

	int idx = Parameter::findParameter(name);

	if (idx<1)
		throw std::runtime_error("Unknown parameter: " + name );

	traces->clearTrace(new Parameter(idx));
}

CmdDelAllTraces::CmdDelAllTraces() :
		Command("delAllTraces") {
}

CmdDelAllTraces::~CmdDelAllTraces() {
}

void CmdDelAllTraces::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << std::endl;
	output << "\tDelete all traces." << std::endl;

}
void CmdDelAllTraces::execute(std::ostream& /*output*/) {
	Traces* traces = Traces::getInstance();

	traces->delAllTraces();
}

CmdClearAllTraces::CmdClearAllTraces() :
		Command("clearAllTraces") {
}

CmdClearAllTraces::~CmdClearAllTraces() {
}

void CmdClearAllTraces::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << std::endl;
	output << "\tClearas all traces." << std::endl;

}
void CmdClearAllTraces::execute(std::ostream& /*output*/) {
	Traces* traces = Traces::getInstance();

	traces->clearAllTraces();
}
