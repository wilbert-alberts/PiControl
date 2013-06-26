/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACES_SERVO_H_
#define TRACES_SERVO_H_

#include "Traces.h"

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <map>
#include <string>

class Traces_Servo: public Traces {
public:

  static void sampleAllTraces(void* db);
  static Traces_Servo* getInstance();

private:
  Traces_Servo(int maxNrTraces);
  int sampleCounter;
};

#endif /* TRACE_H_ */
