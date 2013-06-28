/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#include "CommandProcessor.h"
#include "Command.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

CommandProcessor* CommandProcessor::instance=0;

CommandProcessor* CommandProcessor::getInstance()
{
	if (instance ==0) {
		instance = new CommandProcessor();
	}

	return instance;
}

void CommandProcessor::processCommand(int argc, char* argv[])
{
	std::stringstream args;
	std::string cmd(strrchr(argv[0], '/')+1);
	CommandProcessor* inst = CommandProcessor::getInstance();

	if (cmd == "Terminal") {
		argv++;
		argc--;
		cmd = argv[0];
	}

	if (inst->commands.find(cmd) == inst->commands.end()) {
		throw std::runtime_error("unknown command " + cmd);
	}
	while (argc>0) {
		args << argv[0];
		argv++;
		argc--;
	}
	Command* commando =inst->commands[cmd];
	commando->execute(args);
}

void CommandProcessor::registerCommand(Command* cmd)
{
	commands[cmd->getName()] = cmd;
}

Command* CommandProcessor::lookupCommand(const std::string& cmd)
{
	if (commands.find(cmd) == commands.end())
		return 0;

	return commands[cmd];
}
