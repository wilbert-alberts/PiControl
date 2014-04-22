/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACES_SERVO_H_
#define TRACES_SERVO_H_

#include "Parameter.h"
#include "ServoModule.h"

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <map>
#include <string>

class TraceMsg;

class Traces_Servo: public ServoModule {
public:
	Traces_Servo(ServoModule* predecessor);
	virtual ~Traces_Servo();

	void calculate();

private:
	void reopenStream();
	void sendMessage(TraceMsg* msg);
	void abortStreaming(const std::string& msg);

	int sampleCounter;
	bool streaming;
	int sockfd;

	Parameter* par_sampleCounter;
	Parameter* par_streaming;
};

#endif /* TRACE_H_ */
