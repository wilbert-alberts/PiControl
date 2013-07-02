/*
 * HAL.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: wilbert
 */

#include "SimulatedHAL.h"

#include <stdexcept>


void SimulatedHAL::registerHAL()
{
	HAL::registerHAL(new SimulatedHAL());
}

void SimulatedHAL::setup()
{

}
void SimulatedHAL::pinMode(int pin, int mode)
{

}
int  SimulatedHAL::digitalRead(int pin)
{

}
void SimulatedHAL::digitalWrite(int pin, int value)
{

}
void SimulatedHAL::wiringPiSPIDataRW(int channel, unsigned char *data, int len)
{

}
