/*
 * CmdHelp.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdHelp.h"
#include "CommandProcessor.h"

#include <vector>
#include <string>
#include <iostream>

CmdHelp::CmdHelp() :
		Command("help") {
}

CmdHelp::~CmdHelp() {
	// TODO Auto-generated destructor stub
}

void CmdHelp::execute(std::stringstream& args) {
	std::string c;
	args >> c; // Chop off help command first.
	if (args.eof()) {
		displayHelp();
	} else {
		args >> c;

		Command* cmd = CommandProcessor::getInstance()->lookupCommand(c);
		if (cmd != 0) {
			cmd->displayHelp();
		} else {
			std::clog << "Error, unknown command: " << c << std::endl;
		}
	}
}

void CmdHelp::displayHelp() {
	std::cout << "Usage: Terminal help <cmd>" << std::endl;
	std::cout << "\tAvailable commands:" << std::endl;

	std::vector<Command*> commands;

	CommandProcessor::getInstance()->retrieveAllCommands(commands);

	for (auto i = commands.begin(); i != commands.end(); i++) {
		std::cout << "\t\t" << (*i)->getName()  << std::endl;

	}

}
