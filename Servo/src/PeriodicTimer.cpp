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

#include "PeriodicTimer.h"
#include "Parameter.h"

PeriodicTimer* PeriodicTimer::instance = 0;

PeriodicTimer* PeriodicTimer::getInstance(unsigned int p) {
	if (instance == 0) {
		instance = new PeriodicTimer(p);
	} else {
		if (instance->period != p)
			throw std::runtime_error("Only one PeriodicTimer can be created.");
	}
	return instance;
}

PeriodicTimer* PeriodicTimer::getInstance() {
	if (instance == 0)
		throw std::runtime_error(
				"No PeriodicTimer instance created, creation of PeriodicTimer instance requires period.");
	return instance;
}

PeriodicTimer::PeriodicTimer(unsigned int p) :
		timer_fd(0), period(p), wakeups_missed(0), margin(0), minMargin(0), maxMargin(
				p), stopped(false) {
	par_stopRunning = new Parameter("PeriodicTimer.stopRunning",0);
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
		throw std::system_error(errno, std::system_category(),
				"unable to create timer");
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
		throw std::system_error(errno, std::system_category(),
				"unable to start timer");
	}
}

void PeriodicTimer::start() {

	/* Create the timer */
	stopped = false;
	setupTimer(period);
	while (!stopped) {
		for (std::vector<CallbackContext>::iterator iter = callbacks.begin();
				iter != callbacks.end(); iter++) {
			PeriodicFunction pf = (*iter).first;
			void* context = (*iter).second;
			pf(context);
		}
		updateStats();
		std::cout << "." << std::endl;
		wait();
	}
	close(timer_fd);
}

void PeriodicTimer::checkStop(void* /*context*/) {
	PeriodicTimer* me = PeriodicTimer::getInstance();
	me->stopped = me->par_stopRunning->get() != 0.0;
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
		wakeups_missed += (missed - 1);
}

unsigned int PeriodicTimer::getNrOverruns() {
	return wakeups_missed;
}

double PeriodicTimer::getFrequency() {
	return 1000000.0/period;
}

void PeriodicTimer::updateStats() {
	static struct itimerspec m;
	int ret = timerfd_gettime(timer_fd, &m);
	if (ret == -1) {
		perror("Reading margin");
	}
	margin = m.it_value.tv_nsec / 1000;
	minMargin = margin < minMargin ? margin : minMargin;
	maxMargin = margin > maxMargin ? margin : maxMargin;
}

void PeriodicTimer::updateFrequency(double frequency) {
	unsigned int ns;
	unsigned int sec;
	struct itimerspec itval;

	int p = 1000000/frequency;
	int ret;

	sec = p / 1000000;
	ns = (p - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timerfd_settime(timer_fd, 0, &itval, NULL);
	if (ret == -1) {
		throw std::system_error(errno, std::system_category(),
				"unable to update timer");
	}
	resetStats();
}

unsigned int PeriodicTimer::getTimeElapsed() {
	static struct itimerspec m;
	int ret = timerfd_gettime(timer_fd, &m);
	if (ret == -1) {
		perror("Reading margin");
	}
	return period - m.it_value.tv_nsec / 1000;
}

unsigned int PeriodicTimer::getMargin() {
	return margin;
}

unsigned int PeriodicTimer::getMinMargin() {
	return minMargin;
}

unsigned int PeriodicTimer::getMaxMargin() {
	return maxMargin;
}

unsigned int PeriodicTimer::getPeriod() {
	return period;
}

void PeriodicTimer::resetStats() {
	margin = period;
	minMargin = period;
	maxMargin = 0;
	wakeups_missed = 0;
}
