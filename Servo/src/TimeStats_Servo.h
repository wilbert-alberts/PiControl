/*
 * TimeStats.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_SERVO_H_
#define TIMESTATS_SERVO_H_

#include "TimeStats.h"

#include <string>

class Parameter;
class PeriodicTimer;

class TimeStats_Servo : TimeStats {
public:
  TimeStats_Servo();
  virtual ~TimeStats_Servo();

  static void initSample();
  static void sampleCommand(void* context);
  static void takeTimeStamp(void* context); // context should be Parameter*

private:
  static Parameter* minMargin;
  static Parameter* maxMargin;
  static Parameter* reset;
  static Parameter* margin;
};

#endif /* TIMESTATS_H_ */
