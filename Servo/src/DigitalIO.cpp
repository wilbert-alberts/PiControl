/*
 * DigitalIO.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#include "DigitalIO.h"

#include "DigitalIn.h"
#include "DigitalOut.h"

DigitalIO::DigitalIO(ServoModule* wrapped)
: ServoModule("DIO", wrapped)
{
}

DigitalIO::~DigitalIO() {
}

void DigitalIO::calculateBefore()
{
	DigitalIn::captureAllIns(0);
}

void DigitalIO::calculateAfter()
{
	DigitalOut::activateAllOuts(0);
}

