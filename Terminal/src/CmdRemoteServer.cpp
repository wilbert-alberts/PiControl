/*
 * CmdRemoteServer.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: walberts
 */

#include "CmdRemoteServer.h"

CmdRemoteServer::CmdRemoteServer() {
	// TODO Auto-generated constructor stub

}

CmdRemoteServer::~CmdRemoteServer() {
	// TODO Auto-generated destructor stub
}

void CmdRemoteServer::displayHelp(std::ostream& output) {
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
		std::stringstream outResults
		processor->processCommand(inargs, outResults);
		sendReply(outResults.str());
	}
}

void CmdRemoteServer::setupServer(int port) {
    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
    	throw std::system_error(errno, std::system_category(), "ERROR opening socket");
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = port;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
    	throw std::system_error(errno, std::system_category(), "ERROR opening socket");
    }
    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                                &clilen);
    if (newsockfd < 0)
    {
    	throw std::system_error(errno, std::system_category(), "ERROR on accept");
        exit(1);
    }
}

std::string CmdRemoteServer::getRequest() {
    /* If connection is established then start communicating */
	char [255] lineBuffer;
    bzero(lineBuffer,255);
    n = read( newsockfd,buffer,254 );
    if (n < 0)
    {
    	throw std::system_error(errno, std::system_category(), "ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);
}

void CmdRemoteServer::sendReply(const std::string& reply) {

}


bool CmdServer::exitCommand(const std::string& line) {
	if (line == "exit")
		return true;
}
