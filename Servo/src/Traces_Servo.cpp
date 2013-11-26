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
#include "Traces.h"
#include "DoubleBuffer.h"
#include "Parameter.h"


Traces_Servo* Traces_Servo::instance = 0;

Traces_Servo* Traces_Servo::getInstance()
{
  if (instance == 0) {
    instance = new Traces_Servo();
  }

  return instance;
}

Traces_Servo::Traces_Servo()
: sampleCounter(0)
, par_sampleCounter(new Parameter("Traces.sampleCounter"))
{
	Traces* t = Traces::getInstance();
	t->reset();
}

void Traces_Servo::sampleAllTraces(void* /*context*/)
{
  Traces_Servo* instance = Traces_Servo::getInstance();
  Traces*  traces = Traces::getInstance();

  traces->sample(instance->sampleCounter);
  instance->sampleCounter++;

  instance->par_sampleCounter->set((double)instance->sampleCounter);
}
