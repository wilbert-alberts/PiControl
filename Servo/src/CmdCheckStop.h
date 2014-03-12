/*
 * CmdCheckStop.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#ifndef CMDCHECKSTOP_H_
#define CMDCHECKSTOP_H_

#include "ServoModule.h"

class PeriodicTimer;
class Parameter;

class CmdCheckStop: public ServoModule{
public:
	CmdCheckStop(ServoModule* predecessor);
	virtual ~CmdCheckStop();

	virtual void calculateAfter();

private:
	Parameter* par_stopTimer;
};

#endif /* CMDCHECKSTOP_H_ */
