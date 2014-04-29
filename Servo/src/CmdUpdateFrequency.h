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

class Parameter;
class PeriodicTimer;

class CmdUpdateFrequency : public ServoModule {
public:
	CmdUpdateFrequency(ServoModule* predecessor);
	virtual ~CmdUpdateFrequency();

	void calculateAfter();
	void setPeriodicTimer(PeriodicTimer* _pt);


private:
	Parameter* par_frequency;
	PeriodicTimer* pt;
	double freq;
};

#endif /* CMDUPDATEFREQUENCY_H_ */
