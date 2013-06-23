/*
 * IllagelValException.cpp
 *
 *  Created on: Jun 23, 2013
 *      Author: wilbert
 */

#include "ValException.h"

ValException::ValException(std::string usage)
: reason(std::string("Error: illegal usage\n")+usage)
{
	reason = usage;
}

ValException::~ValException() {
	// TODO Auto-generated destructor stub
}

std::string ValException::msg(){
	return reason;
}
