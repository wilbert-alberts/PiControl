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

CmdClearTrace::CmdClearTrace() :
		Command("clearTrace") {
}

CmdClearTrace::~CmdClearTrace() {
}

void CmdClearTrace::displayHelp() {
	std::cout << "Usage: " << getName() << " <parameter>" << std::endl;
	std::cout << "\tClear trace data for <parameter>." << std::endl;

}
void CmdClearTrace::execute(std::list<std::string>& args) {
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

	traces->clearTrace(new Parameter(idx));
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

CmdClearAllTraces::CmdClearAllTraces() :
		Command("clearAllTraces") {
}

CmdClearAllTraces::~CmdClearAllTraces() {
}

void CmdClearAllTraces::displayHelp() {
	std::cout << "Usage: " << getName() << std::endl;
	std::cout << "\tClearas all traces." << std::endl;

}
void CmdClearAllTraces::execute(std::list<std::string>& args) {
	if (!args.empty()) {
		displayHelp();
		return;
	}

	Traces* traces = Traces::getInstance();

	traces->clearAllTraces();
}
