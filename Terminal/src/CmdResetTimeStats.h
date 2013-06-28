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
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* CMDRESETTIMESTATS_H_ */
