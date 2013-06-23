/*
 * IllagelUsageException.h
 *
 *  Created on: Jun 23, 2013
 *      Author: wilbert
 */

#ifndef ILLAGELUSAGEEXCEPTION_H_
#define ILLAGELUSAGEEXCEPTION_H_

#include "Exception.h"

class UsageException : public Exception{
public:
	UsageException(std::string usage);
	virtual ~UsageException();

	std::string msg();

private:
	std::string reason;
};

#endif /* ILLAGELUSAGEEXCEPTION_H_ */
