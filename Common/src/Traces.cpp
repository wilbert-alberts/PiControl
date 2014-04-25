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

void Trace::clear()
{
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

void Trace::sample(TraceMsg* msg)
{
	if (isSet()) {
		double v = Parameter::getByIdx(parID);

		buffer.pushValue(v);
		msg->addTraceEntry(parID, v);
	}
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

void Traces::clearTrace(Parameter* p)
{
	lock();
	int pID = p->getIndex();
	for (int i=0; i<NRTRACES; i++) {
		if (traces[i].isSet() &&
			(traces[i].getParameterID()) == pID) {
			traces[i].clear();
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

void Traces::clearAllTraces()
{
	lock();
	for (int i=0; i<NRTRACES; i++) {
		traces[i].clear();
	}
	unlock();
}


void Traces::sample(TraceMsg* msg, double sc)
{
	if (tryLock()) {

		for (int i = 0; i < NRTRACES; i++) {
			traces[i].sample(msg);
		}
		sampleCounter = sc;
		unlock();
	}
}

void Traces::dumpTraces(std::ostream& out) {
	lock();

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

	out << "# SampleCounter";
	for (int i=0; i<nrActiveTraces; i++) {
		out << "\t" << Parameter::getNameByIdx(activeTraces[i]->getParameterID());
	}
	out << std::endl;

	int start = sampleCounter - BUFFERSIZE;
	for (int i=0; i<BUFFERSIZE; i++) {
		out << start;
		for (int t=0; t<nrActiveTraces; t++) {
			out << "\t" << destBuffers[t][i];
		}
		out << std::endl;
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

TraceMsg::TraceMsg()
{

}

TraceMsg::~TraceMsg()
{

}

void TraceMsg::addTraceEntry(int parId, double value)
{
	entries[parId] = value;
}

bool TraceMsg::send(int fd)
{
	try {
		sendString("walb");
		sendBody();
		sendString("blaw");

		unsigned int s = write(fd, msgbuf.data(), msgbuf.size());

		if (s != msgbuf.size())
			return false;

		return true;
	}
	catch (std::runtime_error& m) {
		std::clog << "Trace stream lost: " << m.what() << std::endl;
		return false;
	}
	return false;
}

void TraceMsg::sendString(const std::string& msg)
{
	for (auto iter=msg.begin(); iter!=msg.end(); iter++) {
		msgbuf.push_back(*iter);
	}
}

void TraceMsg::sendInt(int v)
{
	unsigned char* p = (unsigned char*)&v;

	for (unsigned int i=0; i<sizeof(int); i++) {
		msgbuf.push_back(p[i]);
	}
}

void TraceMsg::sendDouble(double v)
{
	unsigned char* p = (unsigned char*)&v;

	for (unsigned int i=0; i<sizeof(double); i++) {
		msgbuf.push_back(p[i]);
	}

}

void TraceMsg::sendBody()
{
	sendInt(entries.size());
	for (auto iter = entries.begin(); iter != entries.end(); iter++) {
		int i = iter->first;
		double v = iter->second;

		sendInt(i);
		sendDouble(v);
	}
}

