/*
 * Exception.h
 *
 *  Created on: Jun 23, 2013
 *      Author: wilbert
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>

class Exception {
public:
	Exception();
	virtual ~Exception();

	virtual std::string msg()=0;
};

#endif /* EXCEPTION_H_ */
