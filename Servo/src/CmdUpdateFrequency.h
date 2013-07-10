/*
 * CmdUpdateFrequency.h
 *
 *  Created on: Jul 10, 2013
 *      Author: wilbert
 */

#ifndef CMDUPDATEFREQUENCY_H_
#define CMDUPDATEFREQUENCY_H_

#include <string>

class Parameter;

const std::string PAR_FREQUENCY("PeriodicTimer.frequency");

class Cmd_UpdateFrequency {
public:
	static Cmd_UpdateFrequency* getInstance();
	virtual ~Cmd_UpdateFrequency();

	static void execute(void*);
	void execute();

private:
	Cmd_UpdateFrequency();
	static Cmd_UpdateFrequency* instance;

	double freq;
	Parameter* par_frequency;
};

#endif /* CMDUPDATEFREQUENCY_H_ */
