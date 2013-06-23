/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */


#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <cstdlib>

#include "ValException.h"

#include "Exception.h"
#include "DoubleBuffer.h"
#include "TimeStats_Term.h"
#include "Parameter.h"
#include "StopTimer.h"
#include "Traces_Term.h"


typedef void (*TerminalCommand)(int argc, char* argv[]);
typedef std::map<const std::string, TerminalCommand> CommandMap;

void registerCommands();
void processCommand(int argc, char* argv[]);
void extractCommand(std::string& result, const char* p);
void help(int argc, char* argv[]);

static CommandMap commands;

static const std::string cmdHelp("help");

int main(int argc, char* argv[])
{
	DoubleBuffer* db = DoubleBuffer::getInstance();

	registerCommands();

	try {

		db->connect();
		db->lock();
		processCommand(argc, argv);
	 	db->unlock();
	}
	catch (Exception& ex) {
		std::cerr << ex.msg() << std::endl;
		db->unlock();
	}

	return 0;
}

void registerCommands()
{
	commands[TimeStats_Term::dumpTimingCommand]=&TimeStats_Term::execDumpTiming;
	commands[TimeStats_Term::resetTimingCommand]=&TimeStats_Term::execResetTiming;
	commands[Parameter::dumpAllParametersCommand] = &Parameter::execDumpAllParameters;
	commands[StopTimer::stopTimerCommand] = &StopTimer::execStopTimer;
	commands[Traces_Term::addTraceCommand] = &Traces_Term::execAddTrace;
	commands[Traces_Term::delTraceCommand] = &Traces_Term::execDelTrace;
	commands[Traces_Term::dumpTracesCommand] = &Traces_Term::execDumpTraces;
	commands[cmdHelp] = &help;
}

void processCommand(int argc, char* argv[])
{
	std::string c;
	extractCommand(c, argv[0]);
	if (c == "Terminal") {
		argv++;
		argc--;
		c=argv[0];
	}
	if (commands.find(c) == commands.end()) {
		throw new ValException("Unknown command" + c);
	}

	TerminalCommand cmd = commands[c];

	cmd(argc, argv);
}

void extractCommand(std::string& result, const char* p)
{
	const char* cmd = strrchr(p, '/');

	result.assign(cmd+1);
}
void help(int argc, char* argv[])
{
	std::cout << "Available commands: " << std::endl;
	for (CommandMap::iterator i=commands.begin();
		 i!= commands.end(); i++) {
		std::cout << "\t" << (*i).first << std::endl;
	}
}
