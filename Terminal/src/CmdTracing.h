/*
 * CmdDumpTraces.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDTRACING_H_
#define CMDTRACING_H_

#include "Command.h"

class CmdAddTrace : Command{
public:
	CmdAddTrace();
	virtual ~CmdAddTrace();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

class CmdDelTrace : Command
{
public:
	CmdDelTrace();
	virtual ~CmdDelTrace();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

class CmdDelAllTraces : Command
{
public:
	CmdDelAllTraces();
	virtual ~CmdDelAllTraces();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* CMDADDTRACE_H_ */
