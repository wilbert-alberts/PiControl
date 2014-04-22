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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Traces_Servo.h"
#include "Traces.h"
#include "DoubleBuffer.h"
#include "Parameter.h"


const int   PORTNR   = 9786;
const char* HOSTNAME = "localhost";
const char* TRACEENVVAR = "PITRACER";


Traces_Servo::Traces_Servo(ServoModule* predecessor)
: ServoModule("Tracer", predecessor)
, sampleCounter(0)
, streaming(false)
, sockfd(0)
, par_sampleCounter(createParameter("sampleCounter"))
, par_streaming(createParameter("streaming"))
{
	Traces* t = Traces::getInstance();
	t->reset();
}

Traces_Servo::~Traces_Servo()
{

}

void Traces_Servo::calculate()
{
  Traces*  traces = Traces::getInstance();
  TraceMsg msg;

  reopenStream();
  traces->sample(&msg);
  sampleCounter++;
  sendMessage(&msg);

  *par_sampleCounter = (double)sampleCounter;
}

void Traces_Servo::abortStreaming(const std::string& msg)
{
	std::clog << "Streaming aborted: " << msg << std::endl;
	streaming = false;
	*par_streaming = 0.0;
	if (sockfd > 0) {
		close(sockfd);
		sockfd = 0;
	}
}

void Traces_Servo::reopenStream()
{
	/* Check whether streaming is enabled.
	 * Abort if not enabled.
	 */
	if (streaming && *par_streaming<= 0.0) {
		abortStreaming("Streaming turned off");
		return;
	}

	/* Check whether streaming shoudl be re-enabled */
	if (!streaming && *par_streaming > 0.0) {
	    int portno;
	    struct sockaddr_in serv_addr;
	    struct hostent *server;

	    portno = PORTNR;
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) {
	    	abortStreaming("Unable to open socket");
	    	return;
	    }

	    if (getenv("PITRACER") != NULL) {
	    	server = gethostbyname(getenv(TRACEENVVAR));
	    }
	    else {
	    	server = gethostbyname(HOSTNAME);
	    }
	    if (server == NULL) {
	    	abortStreaming("Unable to resolve hostname");
	    	return;
	    }

	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char *)server->h_addr,
	         (char *)&serv_addr.sin_addr.s_addr,
	         server->h_length);
	    serv_addr.sin_port = htons(portno);
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
	    	abortStreaming("Unable to connect to server");
	    	return;
	    }

	    streaming = true;
	}
}

void Traces_Servo::sendMessage(TraceMsg* msg)
{
	if (streaming) {
		bool r = msg->send(sockfd);
		if (!r)
			abortStreaming("Send trace message failed");
	}
}
