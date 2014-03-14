/*
 * CmdServer.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: walberts
 */

#include "CmdServer.h"

#include <string>
#include <sstream>

CmdServer::CmdServer() {
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

virtual void CmdServer::execute(std::ostream& output)
{
	CommandProcessor* processor = CommandProcessor::getInstance();
	while() {
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

	if (line == "\d")
		return true;
}
