#include <iostream>

#include "DoubleBuffer.h"

#include "PeriodicTimer.h"

#include "Parameter.h"

#include "TimeStats_Servo.h"
#include "Traces_Servo.h"
#include "DigitalOut.h"
#include "DigitalIn.h"
#include "BitBus.h"


void lockDB(void* context);
void unlockDB(void* context);

int main(int /*argc*/, char** /*argv[]*/) {
	try {
		PeriodicTimer* pt;
		DoubleBuffer* db(DoubleBuffer::getInstance());
		Traces_Servo* traces;

		db->create(1024);
		db->lock();

		Parameter* tsStart = new Parameter("TimeStats.start");
		Parameter* tsEnd = new Parameter("TimeStats.end");
		Parameter* tsTracing = new Parameter("TimeStats.tracing");
		Parameter* tsCheckStop = new Parameter("TimeStats.checkStop");

		pt = PeriodicTimer::getInstance(1000000);
		TimeStats_Servo::initSample();
		traces = Traces_Servo::getInstance();
		traces->clearAllTraces();
		db->copyTo();
		db->unlock();

		pt->addPeriodicFunction(lockDB, db);
		pt->addPeriodicFunction(TimeStats_Servo::takeTimeStamp, tsStart);
		// add sample functions
		// Start with capturing digital Is
		pt->addPeriodicFunction(DigitalIn::captureAllIns, 0);

		// Activate digital outs.
		pt->addPeriodicFunction(DigitalOut::activateAllOuts, 0);

		pt->addPeriodicFunction(TimeStats_Servo::takeTimeStamp, tsTracing);
		pt->addPeriodicFunction(Traces_Servo::sampleAllTraces, 0);
		pt->addPeriodicFunction(TimeStats_Servo::sampleCommand, 0);

		pt->addPeriodicFunction(TimeStats_Servo::takeTimeStamp, tsCheckStop);
		pt->addPeriodicFunction(PeriodicTimer::checkStop, 0);

		pt->addPeriodicFunction(TimeStats_Servo::takeTimeStamp, tsEnd);
		pt->addPeriodicFunction(unlockDB, db);

		pt->start();
	} catch (const std::exception& e) {
		std::clog << e.what() << std::endl;
	}
}

void lockDB(void* context) {
	DoubleBuffer* db = static_cast<DoubleBuffer*>(context);
	db->lock();
}

void unlockDB(void* context) {
	DoubleBuffer* db = static_cast<DoubleBuffer*>(context);
	db->unlock();
}
