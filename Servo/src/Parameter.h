/*
 * Parameter.h
 *
 *  Created on: Jun 15, 2013
 *      Author: wilbert
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>

#define TYPE double

class Parameter {
public:
	Parameter(const std::string& name);
	virtual ~Parameter();

	TYPE get();
	void set(TYPE v);
};

#endif /* PARAMETER_H_ */
