/*
 * Trace.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#include <map>
#include <string>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>

#include "Trace.h"

#include "DoubleBuffer.h"
#include "Parameter.h"

std::string Trace::par_startTrace("startTrace");
std::string Trace::par_stopTrace("stopTrace");

std::map<std::string, Trace*> Trace::allTraces;

static const int TRACESIZE = 4096;

Trace::Trace(DoubleBuffer* db, const std::string& par, int s)
: size(s)
{
	int parIdx = Parameter::findParameter(db, par);

	assert(parIdx>=0);

	createMemory(par);
	allTraces[par] = this;
	Parameter* parameter = new Parameter(db, par_startTrace);
	parameter->set(parIdx);
}

Trace::~Trace() {
	// TODO Auto-generated destructor stub
}

void Trace::createMemory(const std::string& name)
{
	std::string shmid("Trace");
	int shmfd;
	int memSize = sizeof(double)*size + sizeof(TracePage);

	shmid += name;
	shmfd = shm_open(shmid.c_str(), O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		perror("Error, unable to create shared memory for trace" );
		exit(-1);
	}

	if (ftruncate(shmfd, memSize)==-1) {
		perror("Error, unable to set length of shared memory of trace");
		exit(-1);
	}

	page = static_cast<TracePage*>(mmap(0, memSize, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0));
}

void Trace::startTraceCommand(DoubleBuffer* db, int argc, char* argv[])
{
	if (argc<=1) {
		for (int i=1; i<argc; i++) {
			if (Parameter::findParameter(db, argv[i])>=0) {
				new Trace(db, std::string(argv[i]), TRACESIZE);
			}
			else {
				std::cerr << "Error: unknown parameter: " << argv[i] << std::endl;
			}
		}
	}
}
