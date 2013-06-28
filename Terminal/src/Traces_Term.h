/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACES_TERM_H_
#define TRACES_TERM_H_

#include "Traces.h"

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <map>
#include <string>

class DoubleBuffer;
class Parameter;

class Traces_Term: public Traces {
public:

  static Traces_Term* getInstance();

  void createTrace(const std::string& par, int size);
  void destroyTrace(const std::string& par);
  void dumpTraces();

  static const int DefaultTraceLength;
private:
  Traces_Term(int nrTraces);
  void attachForRead();
};

#endif /* TRACE_H_ */
