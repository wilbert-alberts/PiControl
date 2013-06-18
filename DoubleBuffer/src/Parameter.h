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

class DoubleBuffer;

class Parameter {
public:
	Parameter(DoubleBuffer* db, const std::string& name);
	virtual ~Parameter();

	std::string getName();
	double get();
	void set(double v);

	static int getNrParameters(DoubleBuffer* db);
	static int findParameter(DoubleBuffer* db, const std::string& name);

private:
	DoubleBuffer* db;
	int idx;
};
#endif /* PARAMETER_H_ */
