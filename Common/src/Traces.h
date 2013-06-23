/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACES_H_
#define TRACES_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <string>
#include <map>

class DoubleBuffer;
class Parameter;
class TraceEntry;

#define MAXNRTRACES (100)

struct TraceDB;

class Traces {
public:
	virtual ~Traces();
	TraceEntry* getTraceEntry(int i);
	int         getNrTraces();

	static const std::string parid_sampleCounter;

	void lockTraceDB();
	void unlockTraceDB();
	bool isTraceDBLocked();

	void clearAllTraces();

	void addTrace(int parIdx, int buffersize);
	void removeTrace(int parIdx);

protected:
	static Traces* traces;

	sem_t*   semDB;     // Manage access to traceDB.
	TraceDB* traceDB;   // To be put in shared memory.
	int      maxNrTraces;

	Parameter* par_sampleCounter;

	void* createSharedMemory(const std::string& id, int size);
	void  createDBSem();

	Traces(int maxNrTraces);

};

#endif /* TRACE_H_ */
