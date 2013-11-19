/*
 * Trace.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#include "Traces_Term.h"
#include "TraceEntry.h"
#include "DoubleBuffer.h"
#include "Parameter.h"

#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

const int Traces_Term::DefaultTraceLength = 1000;

Traces_Term::Traces_Term(int nrTraces)
: Traces(nrTraces)
{}

Traces_Term* Traces_Term::getInstance() {
  if (traces == 0) {
    traces = new Traces_Term(MAXNRTRACES);
  }
  return dynamic_cast<Traces_Term*>(traces);
}

void Traces_Term::createTrace(const std::string& par, int size) {
  int parIdx = Parameter::findParameter(par);
  if (parIdx < 0) {
    throw std::runtime_error("unknown parameter name: " + par);
  }

  if (getNrTraces() >= maxNrTraces) {
    throw std::runtime_error("max nr. of traces exceeded");
  }

  addTrace(parIdx, size);
}

void Traces_Term::destroyTrace(const std::string& par) {
  int parIdx = Parameter::findParameter(par);
  if (parIdx < 0) {
    throw std::runtime_error("unknown parameter name: " + par);
  }

  removeTrace(parIdx);
}

void Traces_Term::dumpTraces() {
  int startCounter;
  int endCounter;
  double v;
  std::ostringstream ss;

  try {
	attachForRead();
    lockTraceDB();
    if (getNrTraces() > 0) {
      startCounter = getTraceEntry(0)->getStart();
      endCounter = getTraceEntry(0)->getEnd();
      ss << "# Samplecounter\t";
      for (int i = 0; i < getNrTraces(); i++) {
        TraceEntry* te = getTraceEntry(i);
        ss << Parameter::getNameByIdx(te->getParameterIndex());
        if (i < getNrTraces() - 1)
          ss << "\t";
        int startOfTrace = te->getStart();
        startCounter =
          startOfTrace < startCounter ?
                         startOfTrace : startCounter;
      }
    }
    unlockTraceDB();

    std::cout << ss.str() << std::endl;

    for (int counter = startCounter; counter < endCounter; counter++) {

      ss.str("");
      ss << counter << "\t";
      for (int i = 0; i < getNrTraces(); i++) {
        TraceEntry* te = getTraceEntry(i);

        lockTraceDB();
        v = te->getSample(counter);
        unlockTraceDB();
        ss << v;
        if (i < getNrTraces() - 1)
          ss << "\t";
      }

      std::cout << ss.str() << std::endl;
    }
  } catch (...) {
    if (isTraceDBLocked())
    {
      unlockTraceDB();
    }
    throw;
  }
}

void Traces_Term::attachForRead() {
  for (int i = 0; i < getNrTraces(); i++) {
    TraceEntry* te = getTraceEntry(i);
    te->attachForRead();
  }
}
