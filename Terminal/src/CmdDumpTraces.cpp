/*
 * CmdDumpTraces.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdDumpTraces.h"

#include "Traces.h"

#include <iostream>
#include <list>
#include <string>

CmdDumpTraces::CmdDumpTraces() :
		Command("dumpTraces") {
}

CmdDumpTraces::~CmdDumpTraces() {
}

void CmdDumpTraces::displayHelp() {
	std::cout << "Usage: " << getName() << std::endl;
	std::cout << "\tDump traced data." << std::endl;

}
void CmdDumpTraces::execute(std::list<std::string>& args) {
	if (!args.empty()) {
		displayHelp();
		return;
	}

	Traces* traces  = Traces::getInstance();

	traces->dumpTraces();
}
