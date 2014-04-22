/*
 * DBLocker.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#ifndef DBLOCKER_H_
#define DBLOCKER_H_

#include "ServoModule.h"

class DoubleBuffer;

class DBLocker: public ServoModule {
public:
	DBLocker(ServoModule* wrapped);
	virtual ~DBLocker();

	virtual void calculateBefore();
	virtual void calculateAfter();

	void setDoubleBuffer(DoubleBuffer* db);

private:
	DoubleBuffer* db;
};

#endif /* DBLOCKER_H_ */
