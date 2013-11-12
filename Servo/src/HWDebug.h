/*
 * HWDebug.h
 *
 *  Created on: Nov 12, 2013
 *      Author: ben
 */

#ifndef HWDEBUG_H_
#define HWDEBUG_H_

#include "Parameter.h"
#include "DigitalOut.h"
#include <string>

class HWDebug {
public:
	virtual ~HWDebug();

	static void raiseEvent(const std::string& msg);

private:
	HWDebug();
	static HWDebug* getInstance();
	static HWDebug* instance;

	Parameter* dumpEvents;
	DigitalOut* dio;
	bool level;
};

#endif /* HWDEBUG_H_ */
