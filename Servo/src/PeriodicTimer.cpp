/*
 * PeriodicTimer.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#include <iostream>
#include <sys/timerfd.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "PeriodicTimer.h"


PeriodicTimer::PeriodicTimer(unsigned int p)
: timer_fd(0), period(p),
  wakeups_missed(0), stopped(false)
{
}

PeriodicTimer::~PeriodicTimer() {

}

void PeriodicTimer::addPeriodicFunction(PeriodicFunction pf, void* context) {
	CallbackContext c(pf, context);
	callbacks.push_back(c);
}

void PeriodicTimer::setupTimer(unsigned int p) {
	int ret;
	unsigned int ns;
	unsigned int sec;
	struct itimerspec itval;

	/* Create the timer */
	timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
	wakeups_missed = 0;
	if (timer_fd == -1) {
		perror("Unable creating timer");
		exit(-1);
	}
	/* Make the timer periodic */
	sec = p / 1000000;
	ns = (p - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timerfd_settime(timer_fd, 0, &itval, NULL);
	if (ret == -1) {
		perror("Unable to start timer");
		exit(-1);
	}
}

void PeriodicTimer::start()
{

	/* Create the timer */
	stopped=false;
	setupTimer(period);
	while (!stopped) {
		for (std::vector<CallbackContext>::iterator iter=callbacks.begin(); iter!=callbacks.end(); iter++) {
			PeriodicFunction pf = (*iter).first;
			void* context = (*iter).second;
			pf(context);
		}
		wait();
	}
	close(timer_fd);
}

void PeriodicTimer::stop()
{
	stopped=true;
}

void PeriodicTimer::wait()
{
	unsigned long long missed;
	int ret;

	/* Wait for the next timer event. If we have missed any the
	   number is written to "missed" */
	ret = read (timer_fd, &missed, sizeof (missed));
	if (ret == -1)
	{
		perror ("read timer");
		return;
	}

	/* "missed" should always be >= 1, but just to be sure, check it is not 0 anyway */
	if (missed > 0)
		wakeups_missed += (missed - 1);
}

unsigned long long PeriodicTimer::getNrOverruns() {
	return wakeups_missed;
}

unsigned long long PeriodicTimer::getMargin() {
	 struct itimerspec m;
	 int ret = timerfd_gettime(timer_fd, &m);
	 if (ret== -1)  {
		 perror("Reading margin");
		 return 0;
	 }
	 return m.it_value.tv_nsec;

}
