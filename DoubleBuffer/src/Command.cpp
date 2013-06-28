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

Command::Command(const std::string& n)
: name(n)
{
	CommandProcessor::getInstance()->registerCommand(this);
}

Command::~Command() {}

const std::string& Command::getName()
{
	return name;
}
