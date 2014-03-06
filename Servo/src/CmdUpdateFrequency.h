/*
 * CmdUpdateFrequency.h
 *
 *  Created on: Jul 10, 2013
 *      Author: wilbert
 */

#ifndef CMDUPDATEFREQUENCY_H_
#define CMDUPDATEFREQUENCY_H_

#include <string>

#include "ServoModule.h"

class Parameter;

class Cmd_UpdateFrequency : public ServoModule {
public:
	static Cmd_UpdateFrequency* getInstance();
	virtual ~Cmd_UpdateFrequency();

	virtual void sample();

private:
	Cmd_UpdateFrequency();
	static Cmd_UpdateFrequency* instance;

	Parameter* par_frequency;
};

#endif /* CMDUPDATEFREQUENCY_H_ */
