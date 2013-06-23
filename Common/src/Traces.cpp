/*
 * Trace.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/types.h>
#include <errno.h>

#include "ValException.h"
#include "PosixException.h"

#include "Traces.h"
#include "TraceEntry.h"

#include "DoubleBuffer.h"
#include "Parameter.h"


#define SEMID_TRACEDB  ("/Traces.traceDB")
#define MEMID_TRACEDB  ("/mem.Traces.traceDB")


Traces* Traces::traces = 0;
const std::string Traces::parid_sampleCounter("Traces.sampleCounter");

typedef struct TraceDB
{
	int 		nrTraces;
	TraceEntry 	entries[0];
} TraceDB;

Traces::~Traces(){

}

Traces::Traces(int nrTraces) {
	int size = sizeof(int) + sizeof(TraceEntry)* nrTraces;
	traceDB = static_cast<TraceDB*>(createSharedMemory(std::string(MEMID_TRACEDB), size));
	maxNrTraces = nrTraces;

	semDB = sem_open(SEMID_TRACEDB, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (semDB == SEM_FAILED ) {
		throw new PosixException(std::string("Unable to create semaphore"), errno);
	}


	par_sampleCounter = new Parameter(Traces::parid_sampleCounter);
}

void Traces::clearAllTraces() {
	memset(traceDB, 0, maxNrTraces);
}


void Traces::lockTraceDB()
{
	int r = sem_wait(semDB);

	if (r!=0) {
		throw new PosixException(std::string("Unable to lock traceDB semaphore"), errno);
	}

	//std::cout << "traces locked" << std::endl;
}

void Traces::unlockTraceDB()
{
	int r = sem_post(semDB);

	if (r!=0) {
		throw new PosixException(std::string("Unable to unlock traceDB semaphore"), errno);
	}
	//std::cout << "traces unlocked" << std::endl;
}

bool Traces::isTraceDBLocked()
{
	int v;
	int r;
	r = sem_getvalue(semDB, &v);
	if (r!=0) {
		throw new PosixException(std::string("Unable to retrieve value of traceDB semaphore"), errno);
	}
	return v!=0;

}

void* Traces::createSharedMemory(const std::string& id, int size)
{
	int shmfd;

	shmfd = shm_open(id.c_str(), O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		throw new PosixException(std::string("Unable to create shared memory for trace"), errno);
	}

	if (ftruncate(shmfd, size)==-1) {
		throw new PosixException(std::string("Unable to set length of shared memory of trace"), errno);
	}

	return mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
}

int Traces::getNrTraces()
{
	return traces->traceDB->nrTraces;
}

TraceEntry* Traces::getTraceEntry(int idx)
{
	if (idx>=traces->traceDB->nrTraces) {
		std::stringstream ss;

		ss << "Trying to get TraceEntry by illegal index: "<<idx;
		throw new ValException(ss.str());
	}

	return &traces->traceDB->entries[idx];
}

void Traces::addTrace(int parIdx, int size)
{
	TraceEntry* te = &traces->traceDB->entries[traces->traceDB->nrTraces];

	std::cout << "> Traces::addTrace" << std::endl;
	te->initialize(parIdx, size);
	std::cout << "> Traces::trace initialized" << std::endl;
	lockTraceDB();
	traceDB->nrTraces++;
	std::cout << "> Traces::nr traces increased" << std::endl;
	unlockTraceDB();
	std::cout << "< Traces::addTrace" << std::endl;
}

void Traces::removeTrace(int parIdx){
	for (int i=0; i<getNrTraces(); i++) {
		TraceEntry* te = getTraceEntry(i);
		if (te->getParameterIndex() == parIdx) {
			void* src = &traceDB->entries[i+1];
			void* dst = &traceDB->entries[i];
			int nrMovedEntries = traceDB->nrTraces - i - 1;

			lockTraceDB();
			memmove(dst, src, nrMovedEntries*sizeof(TraceEntry));
			traceDB->nrTraces--;
			unlockTraceDB();

		}
	}
}
