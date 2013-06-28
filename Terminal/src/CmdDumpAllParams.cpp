/*
 * CmdDumpAllParams.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdDumpAllParams.h"

#include "Parameter.h"

#include <iostream>
#include <string>

CmdDumpAllParams::CmdDumpAllParams() :
		Command("dumpParameters") {
}

CmdDumpAllParams::~CmdDumpAllParams() {
}

void CmdDumpAllParams::displayHelp() {
	std::cout << "Usage: " << getName() << std::endl;
	std::cout << "\tPrint all known parameters and values." << std::endl;

}

void CmdDumpAllParams::execute(std::list<std::string>& args) {
	if (!args.empty()) {
		displayHelp();
	}

	for (int i = 0; i < Parameter::getNrParameters(); i++) {
		std::string parname = Parameter::getNameByIdx(i);
		double value = Parameter::getByIdx(i);
		std::cout << parname << "\t" << value << std::endl;
	}
}

