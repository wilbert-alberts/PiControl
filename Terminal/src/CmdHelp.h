/*
 * CmdHelp.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDHELP_H_
#define CMDHELP_H_

#include "Command.h"

class CmdHelp : Command {
public:
	CmdHelp();
	virtual ~CmdHelp();

	virtual void displayHelp();
	virtual void execute(std::stringstream& args);
};

#endif /* CMDHELP_H_ */
