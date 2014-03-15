/*
 * CmdServer.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: walberts
 */

#include "CmdServer.h"
#include "CommandProcessor.h"

#include <string>
#include <sstream>
#include <iostream>

CmdServer::CmdServer()
: Command("server")
{
	// TODO Auto-generated constructor stub

}

CmdServer::~CmdServer() {
	// TODO Auto-generated destructor stub
}

void CmdServer::displayHelp(std::ostream& out)
{
	out << "Usage: " << getName() << std::endl;
	out << "\tStarts interactive server session over stdin/stdout" << std::endl;
	out << "\tExit by entering 'exit' or an empty line" << std::endl;
}

void CmdServer::execute(std::ostream& output)
{
	CommandProcessor* processor = CommandProcessor::getInstance();
	while(true) {
		char lineBuffer[255];
		std::cin.getline(lineBuffer, 255);
		std::string line(lineBuffer);

		if (exitCommand(line))
			return;

		std::stringstream inargs(line);
		processor->processCommand(inargs, output);
	}
}

bool CmdServer::exitCommand(const std::string& line) {
	if (line == "exit")
		return true;

	if (line.length()==0)
		return true;

	return false;
}
