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

CommandProcessor::CommandProcessor() {}

CommandProcessor::~CommandProcessor() {}

CommandProcessor* CommandProcessor::getInstance()
{
	if (instance ==0) {
		instance = new CommandProcessor();
	}

	return instance;
}

void CommandProcessor::processCommand(int argc, char* argv[])
{
	std::list<std::string> args;
	std::string cmd(strrchr(argv[0], '/')+1);
	CommandProcessor* inst = CommandProcessor::getInstance();

	// In case command is invoked via Terminal, extrace
	// real command by looking to next argument.
	if ((cmd == "Terminal") && (argc ==1)) {
		throw std::runtime_error("Terminal requires at least one argument");
	}

	if (cmd == "Terminal") {
		argv++;
		argc--;
		cmd = argv[0];
	}

	if (inst->commands.find(cmd) == inst->commands.end()) {
		throw std::runtime_error("unknown command " + cmd);
	}

	// Ensure that commandname itself is not
	// added as argument.
	argv++;
	argc--;

	// Create stream holding arguments.
	while (argc>0) {
		args.push_back(argv[0]);
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

void CommandProcessor::retrieveAllCommands(std::vector<Command*>& r)
{
	r.clear();
	for (CommandMap::iterator i = commands.begin(); i!= commands.end(); i++) {
		Command* p;
		p = i->second;
		r.push_back(p);
	}
}
