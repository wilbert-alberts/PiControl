/*
 * CmdDumpAllParams.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdDumpAllParams.h"

#include "DoubleBuffer.h"
#include "Parameter.h"

#include <iostream>
#include <string>

CmdDumpAllParams::CmdDumpAllParams() :
		Command("dumpParameters") {
}

CmdDumpAllParams::~CmdDumpAllParams() {
}

void CmdDumpAllParams::displayHelp(std::ostream& output) {
	output << "Usage: " << getName() << std::endl;
	output << "\tPrint all known parameters and values." << std::endl;

}

void CmdDumpAllParams::execute(std::ostream& out) {
	DoubleBufferLock dbl;
	for (int i = 0; i < Parameter::getNrParameters(); i++) {
		std::string parname = Parameter::getNameByIdx(i);
		double value = Parameter::getByIdx(i);
		out << parname << "\t" << value << std::endl;
	}
}

