/*
 * CmdServer.h
 *
 *  Created on: Mar 14, 2014
 *      Author: walberts
 */

#ifndef CMDSERVER_H_
#define CMDSERVER_H_

#include <Command.h>
#include <string>

class CmdServer: public Command {
public:
	CmdServer();
	virtual ~CmdServer();

	void displayHelp(std::ostream& out);
	virtual void execute(std::ostream& output);

private:
	bool exitCommand(const std::string& line);
};

#endif /* CMDSERVER_H_ */
