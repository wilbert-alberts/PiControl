/*
 * Periodictimer.h
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef PERIODICTIMER_H_
#define PERIODICTIMER_H_

#include <string>
#include <vector>

class DoubleBuffer;
class Parameter;
class Statistics;

class PeriodicTimer;

class PeriodicTimerCB
{
public:
	virtual ~PeriodicTimerCB() {};
	virtual void sample(PeriodicTimer* pt) = 0;
};

class PeriodicTimer {
public:
  PeriodicTimer(double freqInHz);
  virtual ~PeriodicTimer();

  void addCallback(PeriodicTimerCB* cb);

  void start();
  void stop();

  void updateFrequency(double frequency);
  double getFrequency() const;

  unsigned long long getNrOverruns() const;
  double getTime()const ;

private:
  double frequency;
  int timer_fd;
  bool stopped;

  Statistics* marginStats;
  unsigned long long nrOverruns;
  double prevPeriodDuration;


  std::vector<PeriodicTimerCB*> callbacks;

  void wait();
  void setupTimer(double frequency);
  void updateStats();
};

#endif /* PERIODICTIMER_H_ */
