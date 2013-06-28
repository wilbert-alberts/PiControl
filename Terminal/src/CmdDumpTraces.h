/*
 * CmdDumpTraces.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDDUMPTRACES_H_
#define CMDDUMPTRACES_H_

#include "Command.h"

class CmdDumpTraces : Command{
public:
	CmdDumpTraces();
	virtual ~CmdDumpTraces();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* CMDDUMPTRACES_H_ */
