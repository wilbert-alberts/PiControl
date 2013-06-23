/*
 * PosixException.cpp
 *
 *  Created on: Jun 23, 2013
 *      Author: wilbert
 */

#include "string.h"

#include "PosixException.h"

PosixException::PosixException(const std::string& msg, int r)
: reason(msg + std::string(": ") + std::string(strerror(r)))
{
	// TODO Auto-generated constructor stub

}

PosixException::~PosixException() {
	// TODO Auto-generated destructor stub
}

std::string PosixException::msg()
{
	return reason;
}
