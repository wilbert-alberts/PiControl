/*
 * ServoModule.h
 *
 *  Created on: Mar 6, 2014
 *      Author: wilbert
 */

#ifndef SERVOMODULE_H_
#define SERVOMODULE_H_

#include <string>

class Parameter;

class ServoModule {
public:
	ServoModule();
	ServoModule(const std::string& id, int calculationPrio);
	virtual ~ServoModule();

	Parameter* createParameter(const std::string& id);
	Parameter* createParameter(const std::string& id, const double defaultValue);

	virtual void sample();

private:
	std::string id;
};

#endif /* SERVOMODULE_H_ */
