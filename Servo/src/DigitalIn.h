/*
 * DigitalIn.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef DIGITALIN_H_
#define DIGITALIN_H_

#include <map>
#include <string>

class Parameter;

class DigitalIn {
public:
	static DigitalIn* create(const std::string& name, int pin);
	virtual ~DigitalIn();

	int get();

	void setEnabled(bool v);

	static DigitalIn* getByName(const std::string& name);
	static void captureAllIns(void* context);

private:
	DigitalIn(const std::string& name, int pin);

	int pin;
	Parameter* par;
	bool enabled;

	static std::map<const std::string, DigitalIn*> instances;
};

#endif /* DIGITALOUT_H_ */
