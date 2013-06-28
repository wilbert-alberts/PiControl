/*
 * CmdDumpTraces.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDDELTRACE_H_
#define CMDDELTRACE_H_

#include "Command.h"

class CmdDelTrace : Command{
public:
	CmdDelTrace();
	virtual ~CmdDelTrace();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* CMDADDTRACE_H_ */
