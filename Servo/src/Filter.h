/*
 * Filter.h
 *
 *  Created on: Jan 14, 2014
 *      Author: ben
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <string>
#include <deque>

class Parameter;

class Filter {
public:
	Filter(const std::string& id, int dimension);
	virtual ~Filter();

	double calculate(double in);

private:
	const std::string id;
	int dimension;
	Parameter** par_a;
	Parameter** par_b;

	std::deque<double> in;
	std::deque<double> out;
};

class HPFilter {
public:
	HPFilter(const std::string& id, int dimension);
	virtual ~HPFilter();

	double calculate(double in);
private:
	Filter lpf;

};

#endif /* FILTER_H_ */
