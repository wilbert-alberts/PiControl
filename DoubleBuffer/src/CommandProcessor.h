/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef _COMMANDPROCESSOR_H_
#define _COMMANDPROCESSOR_H_

#include <string>
#include <map>
#include <vector>


class Command;
typedef std::map<const std::string, Command*> CommandMap;

class CommandProcessor
{
public:
	static CommandProcessor* getInstance();
	virtual ~CommandProcessor();

	void processCommand(int argc, char* argv[]);
	void registerCommand(Command* cmd);
	Command* lookupCommand(const std::string& cmd);
	void retrieveAllCommands(std::vector<Command*>& commands);

protected:
	static CommandProcessor* instance;
	CommandMap commands;
};

#endif
