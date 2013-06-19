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

#include "DoubleBuffer.h"
#include "DoubleBuffer_Imp.h"
#include "TimeStats.h"

typedef void (*TerminalCommand)(DoubleBuffer* db, int argc, char* argv[]);
typedef std::map<const std::string, TerminalCommand> CommandMap;

void registerCommands();
void processCommand(DoubleBuffer* db, int argc, char* argv[]);

static CommandMap commands;

int main(int argc, char* argv[])
{
	DoubleBuffer* db = new DoubleBuffer_Imp();

	registerCommands();

	db->connect();
	db->lock();

	processCommand(db, argc, argv);

 	db->unlock();

	return 0;
}

void registerCommands()
{
	commands[TimeStats::dumpTimingName]=&TimeStats::execDumpTimingCommand;
	commands[TimeStats::resetTimingName]=&TimeStats::execResetTimingCommand;

}

void processCommand(DoubleBuffer* db, int argc, char* argv[])
{
	std::string c(argv[0]);
	if (true) {
		argv++;
		argc--;
		c=argv[0];
	}
	TerminalCommand cmd = commands[c];

	cmd(db, argc, argv);
}
