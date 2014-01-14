/*
 * CmdDumpTraces.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CmdConfigFilter_H_
#define CmdConfigFilter_H_

#include "Command.h"

class CmdConfigFilter : Command{
public:
	CmdConfigFilter();
	virtual ~CmdConfigFilter();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* CmdConfigFilter_H_ */
