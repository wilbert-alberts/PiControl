/*
 * TimeStats.h
 *
 *  Created on: Jun 16, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_H_
#define TIMESTATS_H_

class PeriodicTimer;
class Parameter;
class DoubleBuffer;

class TimeStats {
public:
	TimeStats(PeriodicTimer* pt, DoubleBuffer* db);
	virtual ~TimeStats();

	static void tick(void*);

private:
	PeriodicTimer*  pt;
	Parameter*      par;
};

#endif /* TIMESTATS_H_ */
