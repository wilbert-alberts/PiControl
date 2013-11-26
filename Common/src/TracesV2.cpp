/*
 * TracesV2.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: wilbert
 */

#include "TracesV2.h"

#include <iostream>
#include <errno.h>


CyclicBuffer::CyclicBuffer()
: nrSamples(0)
, idx(0)
{
}

CyclicBuffer::~CyclicBuffer()
{
}

void CyclicBuffer::reset()
{
	memset(values, 0, sizeof(double)*BUFFERSIZE);
	nrSamples = 0;
	idx = 0;
}

void CyclicBuffer::pushValue(double val)
{
	values[idx]=val;
	idx = (idx+1) % BUFFERSIZE;
	nrSamples++;
}

void CyclicBuffer::getValues(double* dest)
{
	int rt = BUFFERSIZE - idx;
	int lt = idx;

	if (nrSamples<BUFFERSIZE)
		memset(dest, 0, rt*sizeof(double));
	else
		memcpy(dest, values+idx, rt*sizeof(double));
	memcpy(dest+idx, values, lt*sizeof(double));
}


Trace::Trace()
: parameter(0)
{
}

Trace::~Trace()
{
}

void Trace::setParameter(Parameter* p)
{
	if (p != parameter)
		buffer.reset();
	parameter = p;
}

Parameter* Trace::getParameter()
{
	return parameter;
}

void Trace::sample()
{
	if (parameter)
		buffer.pushValue(parameter->get());
}

void Trace::getValues(double* dest)
{
	buffer.getValues(dest);
}


Traces::Traces()
: sampleCounter(new Parameter("Traces.sampleCounter", 0.0))
{
}

Traces::~Traces()
{
}

void Traces::addTrace(Parameter* p)
{
	for (int i=0; i<NRTRACES; i++) {
		if (traces[i].getParameter() == 0) {
			traces[i].setParameter(p);
			return;
		}
	}
	std::clog << "Trace capacity exhausted" << std::endl;
}

void Traces::delTrace(Parameter* p)
{
	for (int i=0; i<NRTRACES; i++) {
		if (traces[i].getParameter() == p) {
			traces[i].setParameter(0);
			return;
		}
	}
	std::clog << "No trace found tracing " << p->getName() << std::endl;
}

void Traces::delTraces()
{
	for (int i=0; i<NRTRACES; i++) {
		traces[i].setParameter(0);
	}
}

void Traces::sample(int samplecounter)
{
	for (int i=0; i<NRTRACES; i++) {
		traces[i].sample();
	}
	sampleCounter->set(samplecounter);
}

void Traces::dumpTraces() {
	double*  destBuffers[100];
	Trace*   activeTraces[100];
	int      nrActiveTraces(0);

	for (int i=0; i<NRTRACES; i++) {
		if (traces[i].getParameter() != 0) {
			activeTraces[i] = &traces[i];
			nrActiveTraces++;
		}
	}

	for (int i=0; i<nrActiveTraces; i++) {
		destBuffers[i] = new double[BUFFERSIZE];
	}

	for (int i=0; i<nrActiveTraces; i++) {
		activeTraces[i]->getValues(destBuffers[i]);
	}

	std::cout << "# SampleCounter";
	for (int i=0; i<nrActiveTraces; i++) {
		std::cout << "\t" << activeTraces[i]->getParameter()->getName();
	}

	int start = sampleCounter->get() - BUFFERSIZE;
	for (int i=0; i<BUFFERSIZE; i++) {
		std::cout << start;
		for (int t=0; t<nrActiveTraces; t++) {
			std::cout << "\t" << destBuffers[t][i];
		}
		start++;
	}
}


void Traces::lock()
{
	int r = pthread_mutex_lock(&mx);
	if (r!=0)
		throw std::system_error(errno, std::system_category(),"unable to lock");
}


bool Traces::tryLock()
{
	int r = pthread_mutex_trylock(&mx);
	if ((r!=0) && (r != EBUSY))
		throw std::system_error(errno, std::system_category(),"unable trying to lock");
	return r == 0;
}


void Traces::unlock()
{
	int r = pthread_mutex_unlock(&mx);
	if (r!=0)
		throw std::system_error(errno, std::system_category(),"unable to lock");
}

