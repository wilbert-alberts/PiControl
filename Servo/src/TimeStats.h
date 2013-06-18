/*
 * TimeStats.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_H_
#define TIMESTATS_H_

#include <string>

class DoubleBuffer;
class Parameter;
class PeriodicTimer;

class TimeStats {
public:
	TimeStats();
	virtual ~TimeStats();

	static void initSample(DoubleBuffer* db, PeriodicTimer* pt);
	static void sampleCommand(void* context);

private:
	static Parameter* minMargin;
	static Parameter* maxMargin;
	static Parameter* reset;
	static Parameter* margin;

	static const std::string par_minMargin;
	static const std::string par_maxMargin;
	static const std::string par_margin;
	static const std::string par_reset;
};

#endif /* TIMESTATS_H_ */
