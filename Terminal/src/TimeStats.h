/*
 * TimeStats.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_H_
#define TIMESTATS_H_

#include <string>

class DoubleBuffer;
class Parameter;
class PeriodicTimer;

class TimeStats {
public:
	TimeStats();
	virtual ~TimeStats();

	static const std::string dumpTimingName;
	static const std::string resetTimingName;

	static void execDumpTimingCommand(DoubleBuffer* db, int argc, char* argv[]);
	static void execResetTimingCommand(DoubleBuffer* db, int argc, char* argv[]);

private:
	static void dumpTiming(DoubleBuffer* db);
	static void resetTiming(DoubleBuffer* db);

};

#endif /* TIMESTATS_H_ */
