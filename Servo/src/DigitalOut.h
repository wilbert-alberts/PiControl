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

class Parameter;

class DigitalOut {
public:
	static DigitalOut* create(const std::string& name, int pin, int value);
	virtual ~DigitalOut();

	void set(int value);
	int get();

	static DigitalOut* getByName(const std::string& name);
	static void activateAllOuts(void* context);

private:
	DigitalOut(const std::string& name, int pin, int value);

	int pin;
	Parameter* par;

	static std::map<const std::string, DigitalOut*> instances;
};

#endif /* DIGITALOUT_H_ */
