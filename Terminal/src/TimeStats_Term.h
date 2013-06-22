/*
 * TimeStats.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_TERM_H_
#define TIMESTATS_TERM_H_

#include <string>

#include "TimeStats.h"

class DoubleBuffer;
class Parameter;
class PeriodicTimer;

class TimeStats_Term: TimeStats {
public:
	TimeStats_Term();
	virtual ~TimeStats_Term();

	static const std::string dumpTimingCommand;
	static const std::string resetTimingCommand;

	static void execDumpTiming(DoubleBuffer* db, int argc, char* argv[]);
	static void execResetTiming(DoubleBuffer* db, int argc, char* argv[]);

private:
	static void dumpTiming(DoubleBuffer* db);
	static void resetTiming(DoubleBuffer* db);

};

#endif /* TIMESTATS_H_ */
