/*
 * Statistics.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#include "Statistics.h"

#include <limits>
#include <cmath>

Statistics::Statistics()
: min(std::numeric_limits<double>::max())
, max(std::numeric_limits<double>::min())
, sum(0.0)
, nrSamples(0)
, sumsum(0.0)
{
}

Statistics::~Statistics() {
	// TODO Auto-generated destructor stub
}

void Statistics::reset() {
	max = std::numeric_limits<double>::min();
	min = std::numeric_limits<double>::max();
	nrSamples = 0;
	sum = 0.0;
	sumsum = 0.0;
}

void Statistics::sample(double v) {
	nrSamples++;
	sum+= v;
	sumsum += (v*v);
	max = v>max ? v: max;
	min = v<min ? v: min;
}

double Statistics::getMin() const {
	return min;
}

double Statistics::getMax() const {
	return max;
}

double Statistics::getAvg() const {
	return sum/nrSamples;
}

double Statistics::getStd() const {
	double sumsumavg = sumsum/nrSamples;
	double sumavg = sum/nrSamples;
	return sqrt( sumsumavg - sumavg*sumavg);

}
