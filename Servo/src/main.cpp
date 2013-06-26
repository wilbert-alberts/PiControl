#include <iostream>

#include "DoubleBuffer.h"

#include "PeriodicTimer.h"

#include "Parameter.h"

#include "TimeStats_Servo.h"
#include "Traces_Servo.h"


void lockDB(void* context);
void unlockDB(void* context);

int main(int /*argc*/, char** /*argv[]*/)
{
  try
  {
    PeriodicTimer* pt;
    DoubleBuffer*  db;
    Traces_Servo*  traces;

    db = DoubleBuffer::getInstance();
    db->create(1024);



    db->lock();
    pt = PeriodicTimer::getInstance(1000000);
    TimeStats_Servo::initSample();
    traces = Traces_Servo::getInstance();
    traces->clearAllTraces();
    db->copyTo();
    db->unlock();

    pt->addPeriodicFunction(lockDB, db);
    pt->addPeriodicFunction(Traces_Servo::sampleAllTraces, 0);
    pt->addPeriodicFunction(TimeStats_Servo::sampleCommand, 0);
    pt->addPeriodicFunction(PeriodicTimer::checkStop, 0);
    pt->addPeriodicFunction(unlockDB, db);

    pt->start();
  }
  catch(const std::exception& e)
  {
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
