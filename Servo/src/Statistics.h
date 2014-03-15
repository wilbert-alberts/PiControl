/*
 * Statistics.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

class Statistics {
public:
	Statistics();
	virtual ~Statistics();

	void reset();
	void sample(double v);
	double getMin() const;
	double getMax() const;
	double getAvg() const;
	double getStd() const;

private:
	double min;
	double max;
	double sum;
	int    nrSamples;
	double sumsum;
};

#endif /* STATISTICS_H_ */
