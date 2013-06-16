/*
 * DoubleBufferImp.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */


#include <iostream>

#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "DoubleBuffer_Imp.h"

static const char* DB_MEM_ID = "/DB.mem";
static const char* DB_LOCK0_ID = "DB.lock0";
static const char* DB_LOCK1_ID = "DB.lock1";


DoubleBuffer_Imp::DoubleBuffer_Imp() {
	// TODO Auto-generated constructor stub

}

DoubleBuffer_Imp::~DoubleBuffer_Imp() {
	// TODO Auto-generated destructor stub
}

void DoubleBuffer_Imp::initSemaphores() {
	semLock0 = sem_open(DB_LOCK0_ID, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (semLock0 == SEM_FAILED ) {
		perror("Error, unable to create semaphore");
		exit(-1);
	}
	semLock1 = sem_open(DB_LOCK1_ID, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
	if (semLock1 == SEM_FAILED ) {
		perror("Error, unable to create semaphore");
		exit(-1);
	}
}

void DoubleBuffer_Imp::create(int size) {
	shmfd = shm_open(DB_MEM_ID, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		perror("Error, unable to create shared memory" );
		exit(-1);
	}

	if (ftruncate(shmfd, (size+sizeof(int))*2)==-1) {
		perror("Error, unable to set length of shared memory");
		exit(-1);
	}

	page0 = mmap(0, (size+sizeof(int))*2, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);

	memset(page0, 0, (size+sizeof(int))*2);

	if (page0 == MAP_FAILED) {
		perror("Error, unable to map shared memory");
		exit(-1);
	}



	char* p1 = static_cast<char*>(page0);
	page1 = static_cast<void*>(p1+size+sizeof(int));
	*((int*)page0)=size;
	*((int*)page1)=size;

	created=true;

	initSemaphores();
}

void DoubleBuffer_Imp::connect() {
	shmfd = shm_open(DB_MEM_ID, O_RDWR, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		perror("Error, unable to create shared memory" );
		exit(-1);
	}

	page0 = mmap(0, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);

	size = ((int*)page0)[0];
	if (ftruncate(shmfd, (size+sizeof(int))*2)==-1) {
		perror("Error, unable to set length of shared memory");
		exit(-1);
	}

	if (page0 == MAP_FAILED) {
		perror("Error, unable to map shared memory");
		exit(-1);
	}

	created=true;

	initSemaphores();
}

void DoubleBuffer_Imp::lock0() {
	// Try to get page0;
	int r = sem_wait(semLock0);
	if (r == -1) {
		perror("Error while locking in sem_wait: ");
	}
	locked0 = true;
}

void DoubleBuffer_Imp::lock1() {
	// Try to get page1
	int r = sem_wait(semLock1);
	if (r == -1) {
		perror("Error while locking in sem_wait: ");
	}
	locked1 = true;
}

void DoubleBuffer_Imp::lock() {
	assert(created);

	// Find out which buffer to lock
	if (buffer == page0) {
		// Try to get page1
		lock1();
		buffer=page1;
	} else {
		// Try to get page0;
		lock0();
		buffer=page0;
	}
}

void DoubleBuffer_Imp::unlock1() {
	locked1 = false;
	int r = sem_post(semLock1);
	if (r == -1) {
		perror("Error unlocking: ");
	}
}

void DoubleBuffer_Imp::unlock0() {
	locked0 = false;
	int r = sem_post(semLock0);
	if (r == -1) {
		perror("Error unlocking: ");
	}
}

void DoubleBuffer_Imp::unlock() {
	assert(created);

	if (locked1) {
		unlock1();
	}
	if (locked0) {
		unlock0();
	}
}

void* DoubleBuffer_Imp::get() {
	assert(created);

	if (!(locked0 || locked1))
		std::cerr << "Error: No buffer locked" << std::endl;
	char* p = static_cast<char*>(buffer);
	p = p+sizeof(int);

	return static_cast<void*>(p);
}

void DoubleBuffer_Imp::copyFrom() {
	char* src;
	char* dst;

	if (!(locked0 || locked1))
		std::cerr << "Error: No buffer locked" << std::endl;
	if (buffer==page0) {
		lock1();
		src= static_cast<char*>(page1);
		dst= static_cast<char*>(page0);
		memcpy (dst, src, size+sizeof(int));
		unlock1();
	}
	else {
		lock0();
		src= static_cast<char*>(page0);
		dst= static_cast<char*>(page1);
		memcpy (dst, src, size+sizeof(int));
		unlock0();
	}
}

void DoubleBuffer_Imp::copyTo() {
	char* src;
	char* dst;

	if (!(locked0 || locked1))
		std::cerr << "Error: No buffer locked" << std::endl;
	if (buffer==page0) {
		lock1();
		src= static_cast<char*>(page0);
		dst= static_cast<char*>(page1);
		memcpy (dst, src, size+sizeof(int));
		unlock1();
	}
	else {
		lock0();
		src= static_cast<char*>(page1);
		dst= static_cast<char*>(page0);
		memcpy (dst, src, size+sizeof(int));
		unlock0();
	}
}
