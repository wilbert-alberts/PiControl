/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>
#include <sstream>

class Command
{
public:
	Command(const std::string& n);
	virtual ~Command();

	const std::string& getName();

	virtual void displayHelp()=0;
	virtual void execute(const std::stringstream& args)=0;

private:
	std::string name;
};

#endif
