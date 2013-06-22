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

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/types.h>


#include "TraceEntry.h"

#include "DoubleBuffer.h"
#include "Parameter.h"

void TraceEntry::initialize(int idx, int size)
{
	parIdx = idx;
	buffersize = size;

	std::string n;
	getValueMemID(n);
	createSharedMemory(n.c_str(), sizeof(double)*size);

}

void TraceEntry::sample(DoubleBuffer* db, int sampleCounter)
{
	if (value==0)
		attachValues();
	lastSampleCounter = sampleCounter;
	double v = Parameter::getByIdx(db, parIdx);
	value[lastSampleIdx] = v;
	lastSampleIdx = (lastSampleIdx+1) % (buffersize);
	nrSamples++;

}

double TraceEntry::getSample(int i)
{
	if (value==0)
		attachValues();
	// Test for sample in the future
	if (i> lastSampleCounter)
		return 0.0;
	// Test for too old sample
	int delta = lastSampleCounter - i;
	if (delta>buffersize )
		return 0.0;

	// delta is in range, return corresponding value.
	int idx = (lastSampleIdx - delta)%buffersize;
	return value[idx];
}

int TraceEntry::getParameterIndex()
{
	return parIdx;
}

int TraceEntry::getStart()
{
	if (nrSamples < buffersize)
		return lastSampleCounter - nrSamples;
	else
		return lastSampleCounter - buffersize;
}

int TraceEntry::getEnd()
{
	return lastSampleCounter;
}

void TraceEntry::getValueMemID(std::string& id)
{
	std::ostringstream c;
	c << MEMID_TRACEVAR << parIdx;
	id.assign(c.str());
}

void TraceEntry::attachValues()
{
	if (value == 0) {
		std::string n;
		getValueMemID(n);
		value = static_cast<double*>(createSharedMemory(n.c_str(), sizeof(double)*buffersize));
	}
}

void* TraceEntry::createSharedMemory(const std::string& id, int size)
{
	int shmfd;

	shmfd = shm_open(id.c_str(), O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		perror("Error, unable to create shared memory for trace" );
		exit(-1);
	}

	if (ftruncate(shmfd, size)==-1) {
		perror("Error, unable to set length of shared memory of trace");
		exit(-1);
	}

	return mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
}




















