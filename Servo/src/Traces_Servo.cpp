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


#include "Traces_Servo.h"
#include "TraceEntry.h"

#include "DoubleBuffer.h"
#include "Parameter.h"


Traces_Servo* Traces_Servo::getInstance(DoubleBuffer* db)
{
	if (traces == 0) {
		traces = new Traces_Servo(db, MAXNRTRACES);
	}

	return dynamic_cast<Traces_Servo*>(traces);
}

Traces_Servo::Traces_Servo(DoubleBuffer* db, int nrTraces)
: Traces(db, nrTraces), sampleCounter(0)
{
}

void Traces_Servo::sampleAllTraces(void* context)
{
	DoubleBuffer* db = static_cast<DoubleBuffer*>(context);
	Traces_Servo* instance = Traces_Servo::getInstance(db);

	instance->lockTraceDB();
	for (int i=0; i<instance->getNrTraces(); i++) {
		TraceEntry* e = instance->getTraceEntry(i);
		e->sample(db, instance->sampleCounter);
	}
	instance->unlockTraceDB();
	instance->par_sampleCounter->set((double)instance->sampleCounter);
	instance->sampleCounter++;
}
