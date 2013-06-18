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
	pageHandles[0].sem = sem_open(DB_LOCK0_ID, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (pageHandles[0].sem == SEM_FAILED ) {
		perror("Error, unable to create semaphore");
		exit(-1);
	}
	pageHandles[1].sem = sem_open(DB_LOCK1_ID, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
	if (pageHandles[1].sem == SEM_FAILED ) {
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

	pageHandles[0].page = mmap(0, (size+sizeof(int))*2, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);

	memset(pageHandles[0].page, 0, (size+sizeof(int))*2);

	if (pageHandles[0].page == MAP_FAILED) {
		perror("Error, unable to map shared memory");
		exit(-1);
	}



	char* p1 = static_cast<char*>(pageHandles[0].page);
	pageHandles[1].page = static_cast<void*>(p1+size+sizeof(int));
	*((int*)pageHandles[0].page)=size;
	*((int*)pageHandles[1].page)=size;

	created=true;

	initSemaphores();
}

void DoubleBuffer_Imp::connect() {
	shmfd = shm_open(DB_MEM_ID, O_RDWR, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		perror("Error, unable to create shared memory" );
		exit(-1);
	}

	pageHandles[0].page = mmap(0, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);

	size = ((int*)pageHandles[0].page)[0];
	if (ftruncate(shmfd, (size+sizeof(int))*2)==-1) {
		perror("Error, unable to set length of shared memory");
		exit(-1);
	}

	if (pageHandles[0].page == MAP_FAILED) {
		perror("Error, unable to map shared memory");
		exit(-1);
	}

	created=true;

	initSemaphores();
}

void DoubleBuffer_Imp::lock(int page) {
	int r = sem_wait(pageHandles[page].sem);
	if (r == -1) {
		perror("Error while locking in sem_wait: ");
	}
	pageHandles[page].locked = true;
}

void DoubleBuffer_Imp::lockAny() {
	int idx;
	
	if (buffer == pageHandles[0].page) {
		// Start with try to lock page 1
		idx = 1;
	}		
	else {
		// Start with try to lock page 1
		idx = 0;
	}
	
	while (1) {
		int r;
		r = sem_trywait(pageHandles[idx].sem);
		if (r==0) {
			pageHandles[idx].locked=true;
			buffer=pageHandles[idx].page;
			return;
		}
		// Try to lock the other page.
		idx = 1-idx;
	}			
}

void DoubleBuffer_Imp::lock() {
	assert(created);

	lockAny();
	return;
}

void DoubleBuffer_Imp::unlock(int page) {
	pageHandles[page].locked = false;
	int r = sem_post(pageHandles[page].sem);
	if (r == -1) {
		perror("Error unlocking: ");
	}
}
void DoubleBuffer_Imp::unlock() {
	assert(created);

	if (pageHandles[1].locked) {
		unlock(1);
		return;
	}
	if (pageHandles[0].locked) {
		unlock(0);
	}
}

void* DoubleBuffer_Imp::get() {
	assert(created);

	if (!(pageHandles[0].locked || pageHandles[1].locked))
		std::cerr << "Error: No buffer locked" << std::endl;
	char* p = static_cast<char*>(buffer);
	p = p+sizeof(int);

	return static_cast<void*>(p);
}

void DoubleBuffer_Imp::copyFrom() {
	char* src;
	char* dst;
	int other;

	if (!(pageHandles[0].locked || pageHandles[1].locked))
		std::cerr << "Error: No buffer locked" << std::endl;

	other = (buffer==pageHandles[0].page) ? 1 : 0;
	lock(other);
	src= static_cast<char*>(pageHandles[other].page);
	dst= static_cast<char*>(pageHandles[1-other].page);
	memcpy (dst, src, size+sizeof(int));
	unlock(other);
}

void DoubleBuffer_Imp::copyTo() {
	char* src;
	char* dst;
	int other;

	if (!(pageHandles[0].locked || pageHandles[1].locked))
		std::cerr << "Error: No buffer locked" << std::endl;

	other = (buffer==pageHandles[0].page) ? 1 : 0;
	lock(other);
	src= static_cast<char*>(pageHandles[1-other].page);
	dst= static_cast<char*>(pageHandles[other].page);
	memcpy (dst, src, size+sizeof(int));
	unlock(other);
}
