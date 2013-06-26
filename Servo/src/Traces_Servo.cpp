/*
 * Trace.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>

#include "Traces_Servo.h"
#include "TraceEntry.h"
#include "DoubleBuffer.h"
#include "Parameter.h"


Traces_Servo* Traces_Servo::getInstance()
{
  if (traces == 0) {
    traces = new Traces_Servo(MAXNRTRACES);
  }

  return dynamic_cast<Traces_Servo*>(traces);
}

Traces_Servo::Traces_Servo(int nrTraces)
: Traces(nrTraces), sampleCounter(0)
{
}

void Traces_Servo::sampleAllTraces(void* /*context*/)
{
  DoubleBuffer* db = DoubleBuffer::getInstance();
  Traces_Servo* instance = Traces_Servo::getInstance();

  instance->lockTraceDB();
  for (int i=0; i<instance->getNrTraces(); i++) {
    TraceEntry* e = instance->getTraceEntry(i);
    e->sample(db, instance->sampleCounter);
  }
  instance->unlockTraceDB();
  instance->par_sampleCounter->set((double)instance->sampleCounter);
  instance->sampleCounter++;
}
