/*
 * IllagelUsageException.cpp
 *
 *  Created on: Jun 23, 2013
 *      Author: wilbert
 */

#include "UsageException.h"

UsageException::UsageException(std::string usage)
: reason(std::string("Error: illegal usage\n")+usage)
{
	reason = usage;
}

UsageException::~UsageException() {
	// TODO Auto-generated destructor stub
}

std::string UsageException::msg(){
	return reason;
}
