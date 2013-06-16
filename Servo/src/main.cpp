#include <iostream>

#include "DoubleBuffer.h"
#include "DoubleBuffer_Imp.h"

#include "PeriodicTimer.h"

#include "Parameter.h"

#include "TimeStats.h"


int main(int argc, char* argv[])
{
	PeriodicTimer* pt;
	DoubleBuffer*  db;
	TimeStats*     ts;

	db = new DoubleBuffer_Imp();
	db->create(1024);

	pt = new PeriodicTimer(1000000);
	ts = new TimeStats(pt, db);

	pt->addPeriodicFunction(lockDB, pt);
	pt->addPeriodicFunction(TimeStats::tick, ts);
	pt->addPeriodicFunction(unlockDB, pt);

	pt->start();
    return 0;
}



void lockDB(void* context) {
	DoubleBuffer* db = static_cast<DoubleBuffer*>(context);
	db->lock();
}

void unlockDB(void* context) {
	DoubleBuffer* db = static_cast<DoubleBuffer*>(context);
	db->unlock();
}
