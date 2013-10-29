/*
 * Trace.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#include "TraceEntry.h"
#include "DoubleBuffer.h"
#include "Parameter.h"

#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/types.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <system_error>

void TraceEntry::initialize(int idx, int size)
{
  parIdx = idx;
  buffersize = size;
}

void TraceEntry::sample(DoubleBuffer* /*db*/, int sampleCounter)
{
  if (wvalue == 0)
    attachForWrite();
  lastSampleCounter = sampleCounter;
  double v = Parameter::getByIdx(parIdx);
  wvalue[lastSampleIdx] = v;
  lastSampleIdx = (lastSampleIdx+1) % (buffersize);
  nrSamples++;
  //std::cout << "Tracing par: " << parIdx << " - " << v << std::endl;
}

void TraceEntry::attachForRead()
{
  std::ostringstream id;
  id << MEMID_TRACEVAR << parIdx;
  rvalue = static_cast<double*>(createSharedMemory(id.str().c_str(), sizeof(double)*buffersize));
}

void TraceEntry::attachForWrite()
{
  std::ostringstream id;
  id << MEMID_TRACEVAR << parIdx;
  wvalue = static_cast<double*>(createSharedMemory(id.str().c_str(), sizeof(double)*buffersize));
}

double TraceEntry::getSample(int i)
{
  // Test for sample in the future
  if (i> lastSampleCounter)
    return 0.0;
  // Test for too old sample
  int delta = lastSampleCounter - i;

  // Either, trace started early enough but sample
  // has already disappeared from buffer or
  // trace is not running long enough.
  if (delta>buffersize || delta > nrSamples)
    return 0.0;

  // delta is in range, return corresponding value.
  //std::cout << "getSample at: " << i << std:: endl;
  //dumpDebug();

  // Note adding buffersize in formula below ensures idx larger then 0.
  int idx = (buffersize + lastSampleIdx - delta)%buffersize;
  return rvalue[idx];
}

void TraceEntry::dumpDebug()
{
  std::cout << "paridx:             " << parIdx << std::endl;
  std::cout << "buffersize:         " << buffersize << std::endl;
  std::cout << "lastSampleCounter:  " << lastSampleCounter << std::endl;
  std::cout << "lastSampleIndex:    " << lastSampleIdx << std::endl;
  std::cout << "nrSamples:          " << nrSamples << std::endl;
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

void* TraceEntry::createSharedMemory(const std::string& id, int size)
{
  int shmfd;

  shmfd = shm_open(id.c_str(), O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
  if (shmfd == -1) {
    throw std::system_error(errno, std::system_category(), "unable to create shared memory for trace");
  }

  if (ftruncate(shmfd, size)==-1) {
    throw std::system_error(errno, std::system_category(), "unable to set length of shared memory of trace");
  }

  return mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
}
