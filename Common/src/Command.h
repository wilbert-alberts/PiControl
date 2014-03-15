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

#include <iostream>

class DoubleBuffer;

class Command {
public:
	Command(const std::string& n);
	virtual ~Command();

	const std::string& getName();

	void perform(std::istream& in, std::ostream& out);

	virtual void displayHelp(std::ostream& output)=0;
	virtual void execute(std::ostream& output)=0;

	bool hasNextArgument();
	std::string getNextArgumentAsString();
	double getNextArgumentAsDouble();

private:
	std::string name;
	bool inExecution;
	std::istream* inArgs;
	std::ostream* outResults;
};

#endif
