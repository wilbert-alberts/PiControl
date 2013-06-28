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

class StopTimer: Command {
public:
	StopTimer();
	virtual ~StopTimer();

	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* STOPTIMER_H_ */
