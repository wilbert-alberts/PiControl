/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>
#include <list>

class Command
{
public:
	Command(const std::string& n);
	virtual ~Command();

	const std::string& getName();

	virtual void displayHelp()=0;
	virtual void execute(std::list<std::string>& args)=0;

private:
	std::string name;
};

#endif
