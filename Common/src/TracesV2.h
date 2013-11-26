/*
 * TracesV2.h
 *
 *  Created on: Nov 23, 2013
 *      Author: wilbert
 */


#include <cstring>
#include <pthread.h>


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


class Trace {
public:
	Trace();
	virtual ~Trace();

	void setParameter(Parameter* par);
	Parameter* getParameter();
	void sample();
	void getValues(double *dest);

private:
	CyclicBuffer buffer;
	int          parameterIndex;
};

class Traces {
public:
	Traces();
	virtual ~Traces();

	void addTrace(Parameter* p);
	void delTrace(Parameter* p);
	void delTraces();

	void sample(int samplecounter);
	void dumpTraces();

private:

	void lock();
	bool tryLock();
	void unlock();

	pthread_mutex_t mx;
	Trace traces[NRTRACES];
	Parameter* sampleCounter;
};


#endif /* TRACESV2_H_ */
