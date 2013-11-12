/*
 * HWDebug.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: ben
 */

#include "HWDebug.h"
#include "DigitalOut.h"
#include "Parameter.h"

#include <iostream>
#include <string>

HWDebug* HWDebug::instance = 0;

HWDebug::HWDebug()
: dumpEvents(new Parameter("Debug.dump"))
, dio(DigitalOut::create("debug", 2,0))
, level(false)
{
	dio->setEnabled(true);
}

HWDebug::~HWDebug() {
	// TODO Auto-generated destructor stub
}

HWDebug* HWDebug::getInstance() {
	if (instance == 0)
		instance = new HWDebug();
	return instance;
}

void HWDebug::raiseEvent(const std::string& msg)
{
	HWDebug* i = HWDebug::getInstance();
	static bool v=0;

	//i->level = !i->level;
	//i->dio->set(i->level);
	i->dio->set(v);
	v = !v;

	if (i->dumpEvents->get()!=0.0)
		std::clog << i->level << msg << std::endl;

}
