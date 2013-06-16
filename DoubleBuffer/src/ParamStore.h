/*
 * ParamStore.h
 *
 *  Created on: Jun 15, 2013
 *      Author: wilbert
 */

#ifndef PARAMSTORE_H_
#define PARAMSTORE_H_

#include <string>

class DoubleBuffer;
class Parameter;

class ParamStore {
public:
	ParamStore(DoubleBuffer* db);
	virtual ~ParamStore();

	Parameter* createParameter(const std::string& name);
	int getNrParameters();
	Parameter* getParameter(int i);

private:
	DoubleBuffer* db;
};


class Parameter {
public:
	Parameter(DoubleBuffer* db, int idx);
	virtual ~Parameter();

	void getName(std::string& name);
	std::string getName();
	double get();
	void set(double v);

private:
	DoubleBuffer* db;
	int idx;
};
#endif /* PARAMSTORE_H_ */
