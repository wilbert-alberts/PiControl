/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACES_TERM_H_
#define TRACES_TERM_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <string>
#include <map>

#include "Traces.h"

class DoubleBuffer;
class Parameter;

class Traces_Term: public Traces {
public:

	static Traces_Term* getInstance();

	void createTrace(const std::string& par, int size);
	void destroyTrace(const std::string& par);
	void dumpTraces();

	static void execAddTrace(int argc, char* argv[]);
	static void execDelTrace(int argc, char* argv[]);
	static void execDumpTraces(int argc, char* argv[]);

	static const std::string addTraceCommand;
	static const std::string delTraceCommand;
	static const std::string dumpTracesCommand;

private:
	Traces_Term(int nrTraces);
	void attachForRead();
};

#endif /* TRACE_H_ */
