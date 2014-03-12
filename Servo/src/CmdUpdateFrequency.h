/*
 * CmdUpdateFrequency.h
 *
 *  Created on: Jul 10, 2013
 *      Author: wilbert
 */

#ifndef CMDUPDATEFREQUENCY_H_
#define CMDUPDATEFREQUENCY_H_

#include "ServoModule.h"

#include <string>

#include "ServoModule.h"

class Parameter;
class PeriodicTimer;

class CmdUpdateFrequency : public ServoModule {
public:
	CmdUpdateFrequency(ServoModule* predecessor);
	virtual ~CmdUpdateFrequency();

	void calculateAfter();

private:
	Parameter* par_frequency;
	double freq;
};

#endif /* CMDUPDATEFREQUENCY_H_ */
