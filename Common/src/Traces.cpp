/*
 * TracesV2.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: wilbert
 */

#include "Traces.h"

#include "Parameter.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <iostream>
#include <system_error>

static const char* TRACING_MEM_ID = "/mem.Tracing";
static const char* SAMPLECOUNTER = "Traces.sampleCounter";

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
	memcpy(dest+rt, values, lt*sizeof(double));
}


Trace::Trace()
: parID(-1)
{
}

Trace::~Trace()
{
}

void Trace::reset()
{
	parID=-1;
	buffer.reset();
}

bool Trace::isSet()
{
	return parID>=0;
}

void Trace::setParameterID(int p)
{
	if (p != parID)
		buffer.reset();
	parID = p;
}

int Trace::getParameterID()
{
	return parID;
}

void Trace::sample()
{
	if (isSet())
		buffer.pushValue(Parameter::getByIdx(parID));
}

void Trace::getValues(double* dest)
{
	buffer.getValues(dest);
}

Traces* Traces::instance= 0;

Traces* Traces::getInstance()
{
	if (instance == 0) {
		int shmfd = shm_open(TRACING_MEM_ID, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
		if (shmfd == -1) {
			throw std::system_error(errno, std::system_category(),"unable to create shared memory");
		}

		int memSize = sizeof(Traces);
		if (ftruncate(shmfd, memSize)==-1) {
			throw std::system_error(errno, std::system_category(),"unable to set length of shared memory");
		}

		instance = static_cast<Traces*>(mmap(0, memSize, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0));
	}
	return instance;
}

Traces::Traces()
{
}

Traces::~Traces()
{
}

void Traces::reset()
{
	sem_init(&semLock, 1, 1);
	delAllTraces();
}

void Traces::addTrace(Parameter* p)
{
	lock();
	int pID = p->getIndex();
	for (int i=0; i<NRTRACES; i++) {
		if (!traces[i].isSet()) {
			traces[i].setParameterID(pID);
			unlock();
			return;
		}
	}
	std::clog << "Trace capacity exhausted" << std::endl;
	unlock();
}

void Traces::delTrace(Parameter* p)
{
	lock();
	int pID = p->getIndex();
	for (int i=0; i<NRTRACES; i++) {
		if (traces[i].isSet() &&
			(traces[i].getParameterID()) == pID) {
			traces[i].reset();
			unlock();
			return;
		}
	}
	std::clog << "No trace found tracing " << p->getName() << std::endl;
	unlock();
}

void Traces::delAllTraces()
{
	lock();
	for (int i=0; i<NRTRACES; i++) {
		traces[i].reset();
	}
	unlock();
}


void Traces::sample(int samplecounter)
{
	static Parameter* p = new Parameter(SAMPLECOUNTER, 0.0);

	if (tryLock()) {
		for (int i = 0; i < NRTRACES; i++) {
			traces[i].sample();
		}
		sampleCounter++;
		p->set(sampleCounter);
		unlock();
	}
}

void Traces::dumpTraces() {
	lock();
	int      parSampleCounterIdx = Parameter::findParameter(SAMPLECOUNTER);

	double*  destBuffers[100];
	Trace*   activeTraces[100];
	int      nrActiveTraces(0);

	for (int i=0; i<NRTRACES; i++) {
		if (traces[i].isSet()) {
			activeTraces[nrActiveTraces] = &traces[i];
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
		std::cout << "\t" << Parameter::getNameByIdx(activeTraces[i]->getParameterID());
	}
	std::cout << std::endl;

	int start = Parameter::getByIdx(parSampleCounterIdx) - BUFFERSIZE;
	for (int i=0; i<BUFFERSIZE; i++) {
		std::cout << start;
		for (int t=0; t<nrActiveTraces; t++) {
			std::cout << "\t" << destBuffers[t][i];
		}
		std::cout << std::endl;
		start++;
	}
	unlock();
}


void Traces::lock()
{
	int r = sem_wait(&semLock);
	if (r!=0)
		throw std::system_error(errno, std::system_category(),"unable to lock");
}


bool Traces::tryLock()
{
	int r = sem_trywait(&semLock);
	if ((r!=0) && (errno != EAGAIN))
		throw std::system_error(errno, std::system_category(),"unable trying to lock");
	return r == 0;
}


void Traces::unlock()
{
	int r = sem_post(&semLock);
	if (r!=0)
		throw std::system_error(errno, std::system_category(),"unable to lock");
}

