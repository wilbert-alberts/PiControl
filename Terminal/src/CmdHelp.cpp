/*
 * CmdHelp.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdHelp.h"
#include "CommandProcessor.h"

#include <list>
#include <string>
#include <iostream>

CmdHelp::CmdHelp() :
		Command("help") {
}

CmdHelp::~CmdHelp() {
	// TODO Auto-generated destructor stub
}

void CmdHelp::execute(std::ostream& output) {
	if (!hasNextArgument()) {
		displayHelp(output);
	} else {
		std::string c = getNextArgumentAsString();

		Command* cmd = CommandProcessor::getInstance()->lookupCommand(c);
		if (cmd != 0) {
			cmd->displayHelp(output);
		} else {
			std::clog << "Error, unknown command: " << c << std::endl;
		}
	}
}

void CmdHelp::displayHelp(std::ostream& output) {
	output << "Usage: Terminal help <cmd>" << std::endl;
	output << "\tAvailable commands:" << std::endl;

	std::vector<Command*> commands;

	CommandProcessor::getInstance()->retrieveAllCommands(commands);

	for (auto i = commands.begin(); i != commands.end(); i++) {
		output << "\t\t" << (*i)->getName() << std::endl;

	}

}
