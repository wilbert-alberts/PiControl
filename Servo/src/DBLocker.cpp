/*
 * DBLocker.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#include "DBLocker.h"

#include "DoubleBuffer.h"

DBLocker::DBLocker(ServoModule* wrapped)
: ServoModule("DBLocker", wrapped)
, db(0)
{
}

DBLocker::~DBLocker() {
}

void DBLocker::setDoubleBuffer(DoubleBuffer* db) {
	this->db = db;
}

void DBLocker::calculateBefore() {
	db->lock();
}

void DBLocker::calculateAfter() {
	db->unlock();
}

