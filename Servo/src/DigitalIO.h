/*
 * DigitalIO.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wilbert
 */

#ifndef DIGITALIO_H_
#define DIGITALIO_H_

#include "ServoModule.h"

class DigitalIO: public ServoModule {
public:
	DigitalIO(ServoModule* wrapped);
	virtual ~DigitalIO();

	virtual void calculateBefore();
	virtual void calculateAfter();

};

#endif /* DIGITALIO_H_ */
