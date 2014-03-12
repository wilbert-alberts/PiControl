/*
 * ITimerConverter.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#include "TimespecConverter.h"

double timespecToSeconds(const struct timespec* tmr)
{
	return tmr->tv_sec + tmr->tv_nsec / 1.0e9;
}

void secondsToTimespec(struct timespec* dest, double seconds)
{
	long secs = (long)seconds;
	seconds -= secs;
	long nsecs = (long)(seconds*1.0e9);

	dest->tv_sec = secs;
	dest->tv_nsec = nsecs;
}
