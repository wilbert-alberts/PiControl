/*
 * CmdRemoteServer.h
 *
 *  Created on: Mar 14, 2014
 *      Author: walberts
 */

#ifndef CMDREMOTESERVER_H_
#define CMDREMOTESERVER_H_

#include <Command.h>

#include <iostream>
#include <string>

#define DEFAULTPORTNUMBER (9988)

class CmdRemoteServer: public Command {
public:
	CmdRemoteServer();
	virtual ~CmdRemoteServer();

	virtual void displayHelp(std::ostream& output);
	virtual void execute(std::ostream& output);

private:
	void setupServer(int port);
	std::string getRequest();
	void sendReply(const std::string& reply);
	bool exitCommand(const std::string& line);

	int clientSockfd;
};

#endif /* CMDREMOTESERVER_H_ */
