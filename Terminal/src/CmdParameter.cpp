/*
 * CmdParameter.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdParameter.h"

#include "Parameter.h"
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

void CmdParameter::displayHelp()
{
	std::cout << "Usage: " << getName() << " [<value>]" << std::endl;
	std::cout << "\tIf no value is given: retrieve value of parameter " << getName() << std::endl;
	std::cout << "\tIf value provided: set parameter "<<getName()<< "to value." << std::endl;
}

void CmdParameter::execute(std::list<std::string>& args)
{
	if (args.empty()) {
		std::cout << par->get() << std::endl;
	}
	else {
		std::stringstream ss(args.front());
		double v;
		ss>>v;

		par->setDeep(v);
	}
}

void CmdParameter::createParameterCommands()
{
	for (int idx=0; idx< Parameter::getNrParameters(); idx++) {
		new CmdParameter(new Parameter(idx));
	}
}
