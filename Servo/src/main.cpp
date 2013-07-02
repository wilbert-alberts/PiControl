#include <iostream>

#include "DoubleBuffer.h"

#include "PeriodicTimer.h"

#include "Parameter.h"

#include "TimeStats_Servo.h"
#include "Traces_Servo.h"
#include "DigitalOut.h"
#include "DigitalIn.h"
#include "SPI.h"
#include "BitBus.h"

#include "HAL.h"
#include "SimulatedHAL.h"
#include "WiringPiHAL.h"

static DigitalOut* hb;

void lockDB(void* context);
void unlockDB(void* context);
void flipper(void* context);

int main(int /*argc*/, char** /*argv[]*/) {

	try {
		PeriodicTimer* pt;
		DoubleBuffer* db(DoubleBuffer::getInstance());
		Traces_Servo* traces;

#ifdef REALMODE
		WiringPiHAL::registerHAL();
#else
		SimulatedHAL::registerHAL();
#endif
		HAL::getInstance()->setup();
		db->create(1024);
		db->lock();

		hb = DigitalOut::create(std::string("hearbeat"), 0,0);

		Parameter* tsStart = new Parameter("TimeStats.start");
		Parameter* tsEnd = new Parameter("TimeStats.end");
		Parameter* tsTracing = new Parameter("TimeStats.tracing");
		Parameter* tsCheckStop = new Parameter("TimeStats.checkStop");

		// TODO: assign channel and pins for BitBus
		BitBus* bitbus = new BitBus(60);

		pt = PeriodicTimer::getInstance(1000000);
		TimeStats_Servo::initSample();
		traces = Traces_Servo::getInstance();
		traces->clearAllTraces();
		db->copyTo();
		db->unlock();

		pt->addPeriodicFunction(lockDB, db);
		pt->addPeriodicFunction(TimeStats_Servo::takeTimeStamp, tsStart);
		// add sample functions
		// Start with capturing digital Ins
		pt->addPeriodicFunction(DigitalIn::captureAllIns, 0);

		// Read Bitbus
		//pt->addPeriodicFunction(SPIu::readBus, bitbus);

		// Run servo
		pt->addPeriodicFunction(flipper,0);

		// Write Bitbus
		pt->addPeriodicFunction(SPI::writeBus, bitbus);

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

void flipper(void* /*context*/)
{
	static int i = 1;
	hb->set(i);
	i=1-i;
	std::cout << i << std::endl;
}

