/*
 * DoubleBufferImp.h
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef DOUBLEBUFFERIMP_H_
#define DOUBLEBUFFERIMP_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>


#include "DoubleBuffer.h"

class DoubleBuffer_Imp: public DoubleBuffer {
public:
	DoubleBuffer_Imp();
	virtual ~DoubleBuffer_Imp();


	virtual void create(int size);
	virtual void connect();

	virtual void lock();
	virtual void unlock();
	virtual void* get();

	virtual void copyFrom();
	virtual void copyTo();


private:
	sem_t* semLock0;
	bool   locked0;
	void*  page0;

	sem_t* semLock1;
	bool   locked1;
	void*  page1;

	int    size;
	void*  buffer;
	int    shmfd;
	bool   created;

	void lock0();
	void lock1();
	void unlock1();
	void unlock0();
	void initSemaphores();
};


#endif /* DOUBLEBUFFERIMP_H_ */
