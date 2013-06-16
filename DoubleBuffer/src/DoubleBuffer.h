/*
 * DoubleBuffer.h
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef DOUBLEBUFFER_H_
#define DOUBLEBUFFER_H_

class DoubleBuffer {
public:
	DoubleBuffer() {};
	virtual ~DoubleBuffer() {};

	virtual void create(int size)=0;
	virtual void connect()=0;

	virtual void lock()=0;
	virtual void unlock()=0;
	virtual void* get()=0;
	virtual void copyFrom()=0;
	virtual void copyTo()=0;

};

#endif /* DOUBLEBUFFER_H_ */
