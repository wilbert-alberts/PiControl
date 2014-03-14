/*
 * DBLocker.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#include "DBLocker.h"

#include "DoubleBuffer.h"

#include <cassert>

DBLocker::DBLocker(ServoModule* wrapped)
: ServoModule("DBLocker", wrapped)
, db(0)
{
	disableTiming();
}

DBLocker::~DBLocker() {
}

void DBLocker::setDoubleBuffer(DoubleBuffer* db) {
	this->db = db;
}

void DBLocker::calculateBefore() {
	assert(db!=0);
	db->lock();
}

void DBLocker::calculateAfter() {
	assert(db!=0);
	db->unlock();
}

