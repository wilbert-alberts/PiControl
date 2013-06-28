/*
 * CmdParameter.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDPARAMETER_H_
#define CMDPARAMETER_H_

#include "Command.h"

class Parameter;

class CmdParameter : Command{
public:
	static void createParameterCommands();
	virtual ~CmdParameter();

	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);

private:
	CmdParameter(Parameter* p);
	Parameter* par;

};

#endif /* CMDPARAMETER_H_ */
