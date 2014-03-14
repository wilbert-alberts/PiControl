/*
 * CmdRemoteServer.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: walberts
 */

#include "CmdRemoteServer.h"
#include "CommandProcessor.h"

#include <sstream>
#include <string>
#include <stdexcept>
#include <system_error>

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

CmdRemoteServer::CmdRemoteServer()
: Command("remoteServer")
{
}

CmdRemoteServer::~CmdRemoteServer() {
	// TODO Auto-generated destructor stub
}

void CmdRemoteServer::displayHelp(std::ostream& out) {
	out << "Usage: " << getName() << " <port>" << std::endl;
	out << "\tStarts interactive server session via <port>" << std::endl;
	out << "\tServer exits after receiving 'exit'" << std::endl;
}

void CmdRemoteServer::execute(std::ostream& output) {
	CommandProcessor* processor = CommandProcessor::getInstance();
	int port(DEFAULTPORTNUMBER);

	if (hasNextArgument())
		port = (int)getNextArgumentAsDouble();

	setupServer(port);
	while (true) {
		std::string line = getRequest();

		if (exitCommand(line))
			return;

		std::stringstream inargs(line);
		std::stringstream outResults;

		processor->processCommand(inargs, outResults);
		sendReply(outResults.str());
	}
}

void CmdRemoteServer::setupServer(int port) {
    /* First call to socket() function */
	int sockfd;
	unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;

    std::clog << "Opening socket" << std::endl;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
    	throw std::system_error(errno, std::system_category(), "ERROR opening socket");
    }
    /* Initialize socket structure */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    /* Now bind the host address using bind() call.*/
    std::clog << "Binding to port" << std::endl;
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
    	throw std::system_error(errno, std::system_category(), "ERROR opening socket");
    }
    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    std::clog << "Start listening" << std::endl;
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    std::clog << "Start accepting" << std::endl;
    clientSockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                                &clilen);
    if (clientSockfd < 0)
    {
    	throw std::system_error(errno, std::system_category(), "ERROR on accept");
        exit(1);
    }
}

std::string CmdRemoteServer::getRequest() {
    /* If connection is established then start communicating */
	char lineBuffer[255] ;
    memset(lineBuffer,0,255);

    char* cursor=lineBuffer;
    int nrRead;
    std::clog << "Start reading" << std::endl;
    nrRead = read( clientSockfd , cursor ,1);
    while ((nrRead>0) && (cursor-lineBuffer<254)) {
    	switch(*cursor) {
    	case 0:
    	case '\n':
    	case '\r':
    		*cursor = 0;
    		nrRead=-1;
    		break;
    	default:
        	cursor++;
            nrRead = read( clientSockfd , cursor,1);
            break;
    	}
    }
    std::clog << "Finished reading" << std::endl;
    std::clog << "Read: " << lineBuffer << std::endl;
    return std::string(lineBuffer);
}

void CmdRemoteServer::sendReply(const std::string& reply) {
	int c = write(clientSockfd, reply.c_str(), reply.length());

	if (c<0) {
    	throw std::system_error(errno, std::system_category(), "ERROR on write");
	}

	if (c<reply.length()) {
    	throw std::system_error(errno, std::system_category(), "Unable to write full reply");
	}
}


bool CmdRemoteServer::exitCommand(const std::string& line) {
	if (line == "exit")
		return true;

	return false;
}
