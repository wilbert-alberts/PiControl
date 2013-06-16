/*
 * Periodictimer.h
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef PERIODICTIMER_H_
#define PERIODICTIMER_H_

#include <vector>

typedef void (*PeriodicFunction)(void* context);

typedef std::pair<PeriodicFunction, void*> CallbackContext;

class PeriodicTimer {
public:
	PeriodicTimer(unsigned int periodInUs);
	virtual ~PeriodicTimer();

	void addPeriodicFunction(PeriodicFunction pf, void* context);

	void start();
	void stop();

	unsigned int getNrOverruns();
	unsigned int getMargin();
	unsigned int getMinMargin();
	void resetStats();

private:
	int timer_fd;
	unsigned int period;
	unsigned int wakeups_missed;
	unsigned int minMargin;
	unsigned int margin;
	bool stopped;

	std::vector<CallbackContext> callbacks;

	void wait();
	void setupTimer(unsigned int p);
	void updateStats();
};

#endif /* PERIODICTIMER_H_ */
