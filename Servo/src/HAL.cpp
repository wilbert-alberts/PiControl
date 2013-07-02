/*
 * HAL.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: wilbert
 */

#include "HAL.h"

#include <stdexcept>

HAL* HAL::instance=0;

HAL::HAL() {}

HAL::~HAL() {}

HAL* HAL::getInstance()
{
	if (instance != 0)
		return instance;
	throw std::runtime_error("No HAL has been registered.");
}

void HAL::registerHAL(HAL* hsi)
{
	if (instance != 0)
		throw std::runtime_error("Only one HAL can be registered.");
	instance = hsi;
}
