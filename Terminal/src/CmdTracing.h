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
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

class CmdDelTrace : Command
{
public:
	CmdDelTrace();
	virtual ~CmdDelTrace();
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

class CmdClearTrace : Command
{
public:
	CmdClearTrace();
	virtual ~CmdClearTrace();
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

class CmdDelAllTraces : Command
{
public:
	CmdDelAllTraces();
	virtual ~CmdDelAllTraces();
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

class CmdClearAllTraces : Command
{
public:
	CmdClearAllTraces();
	virtual ~CmdClearAllTraces();
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

#endif /* CMDADDTRACE_H_ */
