/*
 * TracesV2.h
 *
 *  Created on: Nov 23, 2013
 *      Author: wilbert
 */


#include <cstring>
#include <semaphore.h>
#include <map>
#include <vector>

#include "Parameter.h"

#ifndef TRACESV2_H_
#define TRACESV2_H_



#define BUFFERSIZE 4096
#define NRTRACES   100



class CyclicBuffer {
public:
	CyclicBuffer();
	virtual ~CyclicBuffer();
	void reset();
	void pushValue(double val);
	void getValues(double* dest);

private:
	double values[BUFFERSIZE];
	int    nrSamples;
	int    idx;
};


class TraceMsg
{
public:
	TraceMsg();
	virtual ~TraceMsg();
	void addTraceEntry(int parId, double value);
	bool send(int fd);

private:
	void sendString(const std::string& msg);
	void sendInt(int v);
	void sendDouble(double v);
	void sendBody();
	void sendEntry();

	std::map<int, double> entries;
	std::vector<unsigned char> msgbuf;
};


class Trace {
public:
	Trace();
	virtual ~Trace();

	void reset();
	void clear();

	bool isSet();
	void setParameterID(int id);
	int  getParameterID();
	void sample(TraceMsg* msg);
	void getValues(double *dest);

private:
	CyclicBuffer buffer;
	int          parID;
};

class Traces {
public:
	static Traces* getInstance();
	virtual ~Traces();

	void reset();

	void addTrace(Parameter* p);
	void delTrace(Parameter* p);
	void clearTrace(Parameter* p);
	void delAllTraces();
	void clearAllTraces();

	void sample(TraceMsg* msg, double sampleCounter);
	void dumpTraces(std::ostream& output);

private:
	Traces();

	static Traces* instance;

	void lock();
	bool tryLock();
	void unlock();

	sem_t semLock;
	Trace traces[NRTRACES];
	int   sampleCounter;
};
#endif /* TRACESV2_H_ */
