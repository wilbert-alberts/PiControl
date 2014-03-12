/*
 * DigitalOut.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef DIGITALOUT_H_
#define DIGITALOUT_H_

#include <map>
#include <string>

#include "ServoModule.h"

class Parameter;

class DigitalOut {
public:
	static DigitalOut* create(const std::string& name, int pin, int value);
	virtual ~DigitalOut();

	void set(int value);
	int get();

	void setEnabled(bool v);

	static DigitalOut* getByName(const std::string& name);
	static void activateAllOuts(void* context);

private:
	DigitalOut(const std::string& name, int pin, int value);

	bool enabled;
	int pin;
	Parameter* par;

	static std::map<const std::string, DigitalOut*> instances;
};

class DigitalOuts: public ServoModule
{
public:
	static DigitalOuts* getInstance();
	virtual ~DigitalOuts();

	virtual void sample();
private:
	DigitalOuts();
};

#endif /* DIGITALOUT_H_ */
