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
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

#endif /* TIMESTATS_H_ */
