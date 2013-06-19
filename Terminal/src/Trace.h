/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACE_H_
#define TRACE_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <string>
#include <map>

class DoubleBuffer;

class Trace {
public:
	virtual ~Trace();

	static std::string par_startTrace;
	static std::string par_stopTrace;

	static void startTraceCommand(DoubleBuffer* db, int argc, char* argv[]);
	static void stopTraceCommand(DoubleBuffer* db, int argc, char* argv[]);

private:
	struct TracePage {
		int    lastSampleCounter;
		int    lastSampleIdx;
		int    nrSamples;
		double value[0];
	};


	Trace(DoubleBuffer* db, const std::string& par, int size);

	static std::map<std::string, Trace*> allTraces;

	int    size;
	TracePage*  page;

	void createMemory(const std::string& name);
};

#endif /* TRACE_H_ */
