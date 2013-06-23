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


#include "Traces_Term.h"
#include "TraceEntry.h"

#include "DoubleBuffer.h"
#include "Parameter.h"

const std::string Traces_Term::addTraceCommand("addTrace");
const std::string Traces_Term::delTraceCommand("delTrace");
const std::string Traces_Term::dumpTracesCommand("dumpTraces");

#define DEFAULT_TRACE_LENGTH (1000)

Traces_Term::Traces_Term(int nrTraces)
: Traces(nrTraces)
{
}

Traces_Term* Traces_Term::getInstance()
{
	if (traces == 0) {
		traces = new Traces_Term(MAXNRTRACES);
	}

	return dynamic_cast<Traces_Term*>(traces);
}

void Traces_Term::createTrace(const std::string& par, int size)
{
	int parIdx =Parameter::findParameter(par);
	if(parIdx < 0) {
		std::cerr << "Unable to find parameter: " << par << std::endl;
		return;
	}


	if (getNrTraces()>=maxNrTraces) {
		std::cerr << "Max number of traces exceeded" << std::endl;
		exit(-1);
	}

	addTrace(parIdx, size);
}

void Traces_Term::destroyTrace(const std::string& par)
{
	int parIdx =Parameter::findParameter(par);
	if(parIdx < 0) {
		std::cerr << "Unable to find parameter: " << par << std::endl;
		return;
	}

	removeTrace(parIdx);
}

void Traces_Term::dumpTraces()
{
	int startCounter;
	int endCounter;
	double v;
	std::ostringstream ss;

	lockTraceDB();
	if (getNrTraces() > 0) {
		startCounter = getTraceEntry(0)->getStart();
		endCounter = getTraceEntry(0)->getEnd();
		ss << "Samplecounter\t";
		for (int i = 0; i < getNrTraces(); i++) {
			TraceEntry* te = getTraceEntry(i);
			ss << Parameter::getNameByIdx(te->getParameterIndex());
			if (i < getNrTraces() - 1)
				ss << "\t";
			int startOfTrace = te->getStart();
			startCounter =
					startOfTrace < startCounter ? startOfTrace : startCounter;
		}
	}
	unlockTraceDB();

	std::cout << ss.str() << std::endl;

	for (int counter= startCounter; counter<endCounter; counter++) {

		ss.str("");
		ss << counter << "\t";
		for (int i=0; i<getNrTraces(); i++) {
			TraceEntry* te = getTraceEntry(i);

			lockTraceDB();
			v = te->getSample(counter);
			unlockTraceDB();
			ss << v;
			if (i<getNrTraces()-1)
				ss << "\t";
		}

		std::cout << ss.str() << std::endl;
	}
}


void Traces_Term::execAddTrace(int argc, char* argv[])
{
	Traces_Term* instance = Traces_Term::getInstance();
	for (int i=1; i<argc; i++) {
		std::string par(argv[i]);
		instance->createTrace(par ,DEFAULT_TRACE_LENGTH);
	}
}

void Traces_Term::execDelTrace(int argc, char* argv[])
{
	Traces_Term* instance = Traces_Term::getInstance();
	for (int i=1; i<argc; i++) {
		std::string par(argv[i]);
		instance->destroyTrace(par);
	}
}

void Traces_Term::execDumpTraces(int argc, char* argv[])
{
	if (argc!=1) {
		std::cerr << "Usage: " << dumpTracesCommand << std::endl;
		exit(-1);
	}

	Traces_Term* instance = Traces_Term::getInstance();

	instance->attachForRead();
	instance->dumpTraces();
}

void Traces_Term::attachForRead()
{
	for (int i=0; i<getNrTraces(); i++) {
		TraceEntry* te = getTraceEntry(i);
		te->attachForRead();
	}
}
