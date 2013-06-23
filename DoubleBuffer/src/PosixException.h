/*
 * PosixException.h
 *
 *  Created on: Jun 23, 2013
 *      Author: wilbert
 */

#ifndef POSIXEXCEPTION_H_
#define POSIXEXCEPTION_H_

#include <string>
#include "Exception.h"

class PosixException: public Exception {
public:
	PosixException(const std::string& msg, int r);
	virtual ~PosixException();

	virtual std::string msg();
private:
	std::string reason;
};

#endif /* POSIXEXCEPTION_H_ */
