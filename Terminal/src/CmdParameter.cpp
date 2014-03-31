/*
 * CmdParameter.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdParameter.h"

#include "Parameter.h"
#include "DoubleBuffer.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>

CmdParameter::CmdParameter(Parameter* p):
Command(p->getName()), par(p)
{
}

CmdParameter::~CmdParameter() {
}

void CmdParameter::displayHelp(std::ostream& output)
{
	output << "Usage: " << getName() << " [<value>]" << std::endl;
	output << "\tIf no value is given: retrieve value of parameter " << getName() << std::endl;
	output << "\tIf value provided: set parameter "<<getName()<< "to value." << std::endl;
}

void CmdParameter::execute(std::ostream& output)
{
	DoubleBufferLock dbl;

	if (!hasNextArgument()) {
		output << par->get() << std::endl;
	}
	else {
		double v = getNextArgumentAsDouble();
		par->setDeep(v);
	}
}

void CmdParameter::createParameterCommands()
{
	DoubleBufferLock dbl;
	for (int idx=0; idx< Parameter::getNrParameters(); idx++) {
		new CmdParameter(new Parameter(idx));
	}
}
