/*
 * ITimerConverter.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#ifndef ITIMERCONVERTER_H_
#define ITIMERCONVERTER_H_

#include <sys/timerfd.h>

double timespecToSeconds(const struct timespec* tmr);
void secondsToTimespec(struct timespec* dest, double seconds);

#endif /* ITIMERCONVERTER_H_ */
