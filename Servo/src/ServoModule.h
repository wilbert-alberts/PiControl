/*
 * ServoModule.h
 *
<<<<<<< HEAD
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#ifndef SERVOMODULE_H_
#define SERVOMODULE_H_

#include "PeriodicTimer.h"
#include <string>

class Parameter;
class DigitalIn;
class DigitalOut;

class ServoModule : public PeriodicTimerCB {
public:
	ServoModule(const std::string& id, ServoModule* other);
	virtual ~ServoModule();

	void sample(PeriodicTimer* pt);

	virtual void calculateBefore();
	virtual void calculateAfter();

	const std::string& getId() const {
		return id;
	}

	Parameter* createParameter(const std::string& id);
	Parameter* createParameter(const std::string& id, const double value);
	DigitalIn* createDigitalIn(const std::string& id, int pin);
	DigitalOut* createDigitalOut(const std::string& id, int pin, int value);

	void disableTiming() { measureTiming = false; }
	void enableTiming() { measureTiming = false; }

	PeriodicTimer* getPeriodicTimer() {
		return pt;
	}

private:
	const std::string id;
	PeriodicTimer* pt;

	ServoModule* other;
	Parameter* tsStart1;
	Parameter* tsEnd1;
	Parameter* tsStart2;
	Parameter* tsEnd2;

	bool measureTiming;
};

#endif /* SERVOMODULE_H_ */
