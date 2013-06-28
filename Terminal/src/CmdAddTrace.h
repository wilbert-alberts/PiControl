/*
 * CmdDumpTraces.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDADDTRACE_H_
#define CMDADDTRACE_H_

#include "Command.h"

class CmdAddTrace : Command{
public:
	CmdAddTrace();
	virtual ~CmdAddTrace();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* CMDADDTRACE_H_ */
