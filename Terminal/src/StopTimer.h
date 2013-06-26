/*
 * StopTimer.h
 *
 *  Created on: Jun 22, 2013
 *      Author: wilbert
 */

#ifndef STOPTIMER_H_
#define STOPTIMER_H_

class DoubleBuffer;

class StopTimer {
public:
  StopTimer();
  virtual ~StopTimer();

  static void execStopTimer(int argc, char* argv[]);

  static const std::string stopTimerCommand;
};

#endif /* STOPTIMER_H_ */
