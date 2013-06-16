/*
 * TimeStats.h
 *
 *  Created on: Jun 16, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_H_
#define TIMESTATS_H_

class TimeStats {
public:
	TimeStats(PeriodicTimer* pt,
	        DoubleBuffer*   db);
	virtual ~TimeStats();

	static void tick(void*);

private:
	PeriodicTimer*  pt;
	DoubleBuffer*   db;
	Parameter*      par;};

#endif /* TIMESTATS_H_ */
