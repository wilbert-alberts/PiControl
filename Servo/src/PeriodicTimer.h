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

typedef void (*PeriodicFunction)(void* context);

typedef std::pair<PeriodicFunction, void*> CallbackContext;

class DoubleBuffer;
class Parameter;

class PeriodicTimer {
public:
  static PeriodicTimer* getInstance(unsigned int periodInUs);
  static PeriodicTimer* getInstance();

  virtual ~PeriodicTimer();

  void addPeriodicFunction(PeriodicFunction pf, void* context);

  void start();
  static void checkStop(void* context);

  unsigned int getNrOverruns();
  unsigned int getMargin();
  unsigned int getMinMargin();
  unsigned int getMaxMargin();
  unsigned int getPeriod();
  void resetStats();

  unsigned int getTimeElapsed();

  static const std::string parid_stopRunning;

private:
  PeriodicTimer(unsigned int periodInUs);
  static PeriodicTimer* instance;

  int timer_fd;
  unsigned int period;
  unsigned int wakeups_missed;
  unsigned int margin;
  unsigned int minMargin;
  unsigned int maxMargin;
  bool stopped;

  Parameter* par_stopRunning;

  std::vector<CallbackContext> callbacks;

  void wait();
  void setupTimer(unsigned int p);
  void updateStats();
};

#endif /* PERIODICTIMER_H_ */
