#include <iostream>
#include <functional>

#include "DoubleBuffer.h"

#include "PeriodicTimer.h"

#include "Parameter.h"

#include "Traces_Servo.h"
#include "DigitalIO.h"
#include "DigitalOut.h"
#include "DigitalIn.h"
#include "SPI.h"
#include "BitBus.h"
#include "Controller.h"
#include "Motor.h"
#include "HAL.h"
#include "SimulatedHAL.h"
#include "WiringPiHAL.h"
#include "CmdUpdateFrequency.h"
#include "CmdCheckStop.h"
#include "Devices.h"
#include "HWDebug.h"
#include "DBLocker.h"

constexpr int MEMORYSIZE=64*1024;  // 16 Kilobytes.

int main(int /*argc*/, char** /*argv[]*/) {

	try {
		PeriodicTimer* pt(new PeriodicTimer(1));
		DoubleBuffer* db(DoubleBuffer::getInstance());

#ifdef REALMODE
		WiringPiHAL::registerHAL();
#else
		SimulatedHAL::registerHAL();
#endif
		HAL::getInstance()->setup();

		db->create(MEMORYSIZE);
		db->lock();

		Controller* controller = new Controller(0);
		Motor* motor = new Motor(controller);
		Devices* devices = new Devices(motor);
		SPI* spi = new SPI(devices);
		DigitalIO* dio = new DigitalIO(spi);
		Traces_Servo* tracer = new Traces_Servo(dio);
		CmdCheckStop* cmdCheckStop = new CmdCheckStop (tracer);
		CmdUpdateFrequency* cmdUpdateFreq = new CmdUpdateFrequency(cmdCheckStop);
		DBLocker* dblocker = new DBLocker(cmdUpdateFreq);

		dblocker->setDoubleBuffer(db);
		motor->setDevices(devices);
		controller->setDevices(devices);
		controller->setMotor(motor);
		devices->setSPI(spi);

		pt->addCallback(dblocker);

		db->unlock();
		pt->start();
	} catch (const std::exception& e) {
		std::clog << e.what() << std::endl;
	}
}



