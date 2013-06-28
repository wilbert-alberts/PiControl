/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#include "DoubleBuffer.h"
#include "TimeStats_Term.h"
#include "Parameter.h"
#include "StopTimer.h"
#include "Traces_Term.h"

#include "CommandProcessor.h"
#include "CmdHelp.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

void registerCommands();

int main(int argc, char* argv[])
{
  DoubleBuffer* db = DoubleBuffer::getInstance();

  registerCommands();

  try {
    db->connect();
    db->lock();
    CommandProcessor::getInstance()->processCommand(argc, argv);
    db->unlock();
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    db->unlock();
  }

  return 0;
}

void registerCommands()
{/*
  commands[TimeStats_Term::dumpTimingCommand]=&TimeStats_Term::execDumpTiming;
  commands[TimeStats_Term::resetTimingCommand]=&TimeStats_Term::execResetTiming;
  commands[Parameter::dumpAllParametersCommand] = &Parameter::execDumpAllParameters;
  commands[StopTimer::stopTimerCommand] = &StopTimer::execStopTimer;
  commands[Traces_Term::addTraceCommand] = &Traces_Term::execAddTrace;
  commands[Traces_Term::delTraceCommand] = &Traces_Term::execDelTrace;
  commands[Traces_Term::dumpTracesCommand] = &Traces_Term::execDumpTraces;
  */
  new CmdHelp();
}
