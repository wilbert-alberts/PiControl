/*
 * TimeStats.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_SERVO_H_
#define TIMESTATS_SERVO_H_

#include <string>

#include "TimeStats.h"

class Parameter;
class PeriodicTimer;

class TimeStats_Servo : TimeStats {
public:
	TimeStats_Servo();
	virtual ~TimeStats_Servo();

	static void initSample();
	static void sampleCommand(void* context);

private:
	static Parameter* minMargin;
	static Parameter* maxMargin;
	static Parameter* reset;
	static Parameter* margin;
};

#endif /* TIMESTATS_H_ */
