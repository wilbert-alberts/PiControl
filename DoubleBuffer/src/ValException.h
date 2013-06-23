/*
 * IllagelUsageException.h
 *
 *  Created on: Jun 23, 2013
 *      Author: wilbert
 */

#ifndef VALEXCEPTION_H_
#define VALEXCEPTION_H_

#include "Exception.h"

class ValException : public Exception{
public:
	ValException(std::string usage);
	virtual ~ValException();

	std::string msg();

private:
	std::string reason;
};

#endif /* ILLAGELUSAGEEXCEPTION_H_ */
