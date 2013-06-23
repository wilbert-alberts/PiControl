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

	static Traces_Term* getInstance(DoubleBuffer* db);

	void createTrace(DoubleBuffer* db, const std::string& par, int size);
	void destroyTrace(DoubleBuffer* db, const std::string& par);
	void dumpTraces(DoubleBuffer* db);

	static void execAddTrace(DoubleBuffer* db, int argc, char* argv[]);
	static void execDelTrace(DoubleBuffer* db, int argc, char* argv[]);
	static void execDumpTraces(DoubleBuffer* db, int argc, char* argv[]);

	static const std::string addTraceCommand;
	static const std::string delTraceCommand;
	static const std::string dumpTracesCommand;

private:
	Traces_Term(DoubleBuffer* db, int nrTraces);
	void attachForRead();
};

#endif /* TRACE_H_ */
