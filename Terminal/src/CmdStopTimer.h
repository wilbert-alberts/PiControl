/*
 * StopTimer.h
 *
 *  Created on: Jun 22, 2013
 *      Author: wilbert
 */

#ifndef STOPTIMER_H_
#define STOPTIMER_H_

#include "Command.h"

#include <list>
#include <string>

class Cmd_StopTimer: Command {
public:
	Cmd_StopTimer();
	virtual ~Cmd_StopTimer();

	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);
};

#endif /* STOPTIMER_H_ */
