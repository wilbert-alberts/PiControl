/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#include "DoubleBuffer.h"
#include "Parameter.h"
#include "StopTimer.h"
#include "Traces_Term.h"

#include "CommandProcessor.h"
#include "CmdHelp.h"
#include "CmdDumpTraces.h"
#include "CmdAddTrace.h"
#include "CmdDelTrace.h"
#include "CmdResetTimeStats.h"
#include "CmdDumpTiming.h"
#include "CmdDumpAllParams.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

void registerCommands();

int main(int argc, char* argv[]) {
	DoubleBuffer* db = DoubleBuffer::getInstance();

	registerCommands();

	try {
		db->connect();
		db->lock();
		CommandProcessor::getInstance()->processCommand(argc, argv);
		db->unlock();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		db->unlock();
	}

	return 0;
}

void registerCommands() {
	new CmdHelp();
	new StopTimer();
	new CmdDumpTraces();
	new CmdAddTrace();
	new CmdDelTrace();
	new CmdResetTimeStats();
	new CmdDumpTiming();
	new CmdDumpAllParams();
}
