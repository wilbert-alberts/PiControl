/*
 * TimeStats.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_TERM_H_
#define TIMESTATS_TERM_H_

#include "Command.h"

#include <string>

class CmdDumpTiming: Command {
public:
	CmdDumpTiming();
	virtual ~CmdDumpTiming();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* TIMESTATS_H_ */
