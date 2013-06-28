/*
 * CmdDumpAllParams.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CMDDUMPALLPARAMS_H_
#define CMDDUMPALLPARAMS_H_

#include "Command.h"

class CmdDumpAllParams : Command{
public:
	CmdDumpAllParams();
	virtual ~CmdDumpAllParams();
	virtual void displayHelp();
	virtual void execute(std::list<std::string>& args);
};

#endif /* CMDDUMPALLPARAMS_H_ */
