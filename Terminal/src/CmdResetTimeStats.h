/*
 * CmdResetTimeStats.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDRESETTIMESTATS_H_
#define CMDRESETTIMESTATS_H_

#include "Command.h"

class CmdResetTimeStats: Command {
public:
	CmdResetTimeStats();
	virtual ~CmdResetTimeStats();
	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

#endif /* CMDRESETTIMESTATS_H_ */
