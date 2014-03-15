/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#include "Command.h"
#include "CommandProcessor.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cassert>

Command::Command(const std::string& n)
: name(n)
, inExecution(false)
, inArgs(0)
, outResults(0)
{
	CommandProcessor::getInstance()->registerCommand(this);
}

Command::~Command() {}

const std::string& Command::getName()
{
	return name;
}

bool Command::hasNextArgument() {
	assert(inExecution);
	if (!inArgs->good())
		return false;
	if (inArgs->eof())
		return false;

	while (inArgs->good()) {
		switch (inArgs->peek())
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			inArgs->get();
			break;
		default:
			return true;
		}
	}
	return false;
}

std::string Command::getNextArgumentAsString() {
	assert(inExecution);
	if (!hasNextArgument()) {
		throw std::runtime_error("No argument found");
	}
	std::string r;
	*inArgs >> r;
	return r;
}

double Command::getNextArgumentAsDouble() {
	assert(inExecution);

	if (!hasNextArgument()) {
		throw std::runtime_error("No argument found");
	}
	assert(hasNextArgument());
	double r;
	*inArgs >> r;
	if (inArgs->fail()) {
		throw std::runtime_error("Unable to parse number");
	}
	return r;
}

void Command::perform(std::istream& in, std::ostream& out) {
	inArgs = &in;
	outResults = &out;
	inExecution = true;
	try {
		execute(*outResults);
	}
	catch (std::runtime_error& ex) {
		displayHelp(out);
	}
	inExecution = false;
	inArgs = 0;
	outResults = 0;
}

