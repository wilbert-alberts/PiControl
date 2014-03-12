/*
 * PeriodicTimer.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#include <sys/timerfd.h>
#include <unistd.h>
#include <errno.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <stdexcept>

#include "TimespecConverter.h"
#include "PeriodicTimer.h"
#include "Statistics.h"

PeriodicTimer::PeriodicTimer(double f)
: frequency(f)
, timer_fd(0)
, stopped(false)
, marginStats(new Statistics())
, nrOverruns(0)
, prevPeriodDuration(0.0)
{
	marginStats = new Statistics();
}

PeriodicTimer::~PeriodicTimer() {
	delete marginStats;
}

void PeriodicTimer::addCallback(PeriodicTimerCB* cb) {
	callbacks.push_back(cb);
}

void PeriodicTimer::setupTimer(double frequency) {
	int ret;
	double period = 1.0/frequency;
	struct timespec timespec;
	struct itimerspec timerspec;

	/* Reset timer statistics */
	marginStats->reset();

	/* Create the timer */
	timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (timer_fd == -1) {
		throw std::system_error(errno, std::system_category(),
				"unable to create timer");
	}

	secondsToTimespec(&timespec, period);
	timerspec.it_interval = timespec;
	timerspec.it_value = timespec;

	/* Make the timer periodic */
	ret = timerfd_settime(timer_fd, 0, &timerspec, NULL);
	if (ret == -1) {
		throw std::system_error(errno, std::system_category(),
				"unable to start timer");
	}
}

double PeriodicTimer::getTime() const {
	int ret;
	struct itimerspec timerspec;
	double period = 1.0/frequency;

	ret = timerfd_gettime(timer_fd, &timerspec);
	if (ret == -1) {
		perror("getTime");
		return -1.0;
	}
	return period - timespecToSeconds(&timerspec.it_value);
}

void PeriodicTimer::start() {

	stopped = false;
	marginStats->reset();
	setupTimer(frequency);
	while (!stopped) {
		for (std::vector<PeriodicTimerCB*>::iterator iter = callbacks.begin();
				iter != callbacks.end(); iter++) {
			(*iter)->sample(this);
		}
		marginStats->sample(getTime());
		prevPeriodDuration = getTime();
		wait();
	}
	close(timer_fd);
}

void PeriodicTimer::stop()
{
	stopped = true;
}

void PeriodicTimer::wait() {
	unsigned long long missed;
	int ret;

	/* Wait for the next timer event. If we have missed any the
	 number is written to "missed" */
	ret = read(timer_fd, &missed, sizeof(missed));
	if (ret == -1) {
		perror("read timer");
		return;
	}

	/* "missed" should always be >= 1, but just to be sure, check it is not 0 anyway */
	if (missed > 0)
		nrOverruns += (missed - 1);
}

unsigned long long PeriodicTimer::getNrOverruns() const {
	return nrOverruns;
}

double PeriodicTimer::getFrequency() const {
	return frequency;
}

void PeriodicTimer::updateStats() {
	double time = getTime();

	marginStats->sample(time);
}

void PeriodicTimer::updateFrequency(double frequency) {
	struct timespec ts;
	struct itimerspec itval;

	double period = 1.0 / frequency;
	int ret;

	marginStats->reset();
	if (timer_fd != 0) {
		secondsToTimespec(&ts, period);
		itval.it_interval = ts;
		itval.it_value = ts;

		ret = timerfd_settime(timer_fd, 0, &itval, NULL);
		if (ret == -1) {
			throw std::system_error(errno, std::system_category(),
					"unable to update timer");
		}
	}
}


