/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACES_SERVO_H_
#define TRACES_SERVO_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <string>
#include <map>

#include "Traces.h"

class Traces_Servo: public Traces {
public:

	static void sampleAllTraces(void* db);
	static Traces_Servo* getInstance(DoubleBuffer* db);

private:
	Traces_Servo(DoubleBuffer* db, int maxNrTraces);
	int sampleCounter;
};

#endif /* TRACE_H_ */
