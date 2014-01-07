/*
 * Trace.h
 *
 *  Created on: Jun 19, 2013
 *      Author: wilbert
 */

#ifndef TRACES_SERVO_H_
#define TRACES_SERVO_H_

#include "Parameter.h"

#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#include <map>
#include <string>

class TraceMsg;

class Traces_Servo{
public:

  static void sampleAllTraces(void* db);
  static Traces_Servo* getInstance();

private:
  Traces_Servo();
  void reopenStream();
  void sendMessage(TraceMsg* msg);
  void abortStreaming(const std::string& msg);

  static Traces_Servo* instance;

  int        sampleCounter;
  bool       streaming;
  int        sockfd;

  Parameter* par_sampleCounter;
  Parameter* par_streaming;
};

#endif /* TRACE_H_ */
