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

typedef struct DoubleBufferPage
{
	int   pagesize;
	void* mem[0];
} DoubleBufferPage;

typedef struct PageHandle {
		sem_t* sem;
		bool   locked;
		DoubleBufferPage*  page;
} PageHandle ;

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

	PageHandle pageHandles[2];

	DoubleBufferPage*  buffer;
	int    shmfd;
	bool   created;

	void lockAny();
	void lock(int page);
	void unlock(int page);
	void initSemaphores();
};


#endif /* DOUBLEBUFFERIMP_H_ */
