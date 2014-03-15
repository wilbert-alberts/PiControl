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
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

#endif /* CMDDUMPTRACES_H_ */
