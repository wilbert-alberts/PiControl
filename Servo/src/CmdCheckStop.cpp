/*
 * CmdCheckStop.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#include "CmdCheckStop.h"
#include "PeriodicTimer.h"
#include "Parameter.h"

#include <assert.h>

CmdCheckStop::CmdCheckStop(ServoModule* predecessor)
: ServoModule("CmdCheckStop", predecessor)
, par_stopTimer(createParameter("stopRunning"))
{
	// TODO Auto-generated constructor stub

}

CmdCheckStop::~CmdCheckStop() {
	// TODO Auto-generated destructor stub
}


void CmdCheckStop::calculateAfter() {
	if (*par_stopTimer != 0.0)
		getPeriodicTimer()->stop();
}
