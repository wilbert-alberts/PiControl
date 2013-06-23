#include <iostream>

#include "DoubleBuffer.h"
#include "DoubleBuffer_Imp.h"

#include "PeriodicTimer.h"

#include "Parameter.h"

#include "TimeStats_Servo.h"
#include "Traces_Servo.h"


void lockDB(void* context);
void unlockDB(void* context);

int main(int argc, char* argv[])
{
	PeriodicTimer* pt;
	DoubleBuffer*  db;
	Traces_Servo*  traces;

	db = new DoubleBuffer_Imp();
	db->create(1024);



	db->lock();
	pt = new PeriodicTimer(db,1000000);
	TimeStats_Servo::initSample(db,pt);
	traces = Traces_Servo::getInstance(db);
	traces->clearAllTraces();
	db->copyTo();
	db->unlock();

	pt->addPeriodicFunction(lockDB, db);
	pt->addPeriodicFunction(Traces_Servo::sampleAllTraces, db);
	pt->addPeriodicFunction(TimeStats_Servo::sampleCommand, pt);
	pt->addPeriodicFunction(PeriodicTimer::checkStop, pt);
	pt->addPeriodicFunction(unlockDB, db);

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
