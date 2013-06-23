/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACEENTRY_H_
#define TRACEENTRY_H_


#define MEMID_TRACEVAR ("/mem.Traces.var.")

class DoubleBuffer;

class TraceEntry {
public:
	virtual ~TraceEntry() {};
	void     initialize(int parIdx, int buffersize);
	void     sample(DoubleBuffer* db, int sampleCounter);
	double   getSample(int sampleCounter);
	int      getParameterIndex();
	int      getStart();
	int      getEnd();

	void 	 attachForRead();

protected:

private:
	void*    createSharedMemory(const std::string& id, int size);
	void     getValueMemID(std::string& id);
	void 	 attachForWrite();

	void     dumpDebug();

	// Set by Terminal
	int     parIdx;
	int     buffersize;

	// Set by Servo
	int     lastSampleCounter;
	int     lastSampleIdx;
	int     nrSamples;

	double* wvalue;
	double* rvalue;
};

#endif /* TRACE_H_ */
