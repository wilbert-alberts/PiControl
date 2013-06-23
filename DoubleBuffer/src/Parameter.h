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
	Parameter(const std::string& name);
	virtual ~Parameter();

	std::string getName();
	double get();
	void set(double v);

	static const std::string dumpAllParametersCommand;
	static void execDumpAllParameters(int argc, char* argv[]);

	static int getNrParameters();
	static int findParameter(const std::string& name);

	static void setByIdx(int i, double v);
	static double getByIdx(int i);
	static std::string getNameByIdx(int i);

private:
	DoubleBuffer* db;
	int idx;
};
#endif /* PARAMETER_H_ */
