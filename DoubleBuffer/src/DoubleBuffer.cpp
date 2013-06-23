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
#include <errno.h>

#include "PosixException.h"
#include "DoubleBuffer.h"

static const char* DB_MEM_ID = "/mem.DB.pages";
static const char* DB_LOCK0_ID = "/DB.lock0";
static const char* DB_LOCK1_ID = "/DB.lock1";

DoubleBuffer* DoubleBuffer::instance = 0;

DoubleBuffer::DoubleBuffer() {
	// TODO Auto-generated constructor stub

}

DoubleBuffer::~DoubleBuffer() {
	// TODO Auto-generated destructor stub
}

DoubleBuffer* DoubleBuffer::getInstance()
{
	if (instance==0) {
		instance = new DoubleBuffer();
	}
	return instance;
}

void DoubleBuffer::initSemaphores() {
	pageHandles[0].sem = sem_open(DB_LOCK0_ID, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (pageHandles[0].sem == SEM_FAILED ) {
		throw new PosixException(std::string("Unable to create semaphore"), errno);
	}
	pageHandles[1].sem = sem_open(DB_LOCK1_ID, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
	if (pageHandles[1].sem == SEM_FAILED ) {
		throw new PosixException(std::string("Unable to create semaphore"), errno);
	}
}

void DoubleBuffer::create(int size) {
	shmfd = shm_open(DB_MEM_ID, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		throw new PosixException(std::string("Unable to create shared memory"), errno);
	}

	int memSize = (size+sizeof(DoubleBufferPage))*2;
	if (ftruncate(shmfd, memSize)==-1) {
		throw new PosixException(std::string("Unable to set length of shared memory"), errno);
	}

	pageHandles[0].page = static_cast<DoubleBufferPage*>(mmap(0, memSize, PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0));

	memset(pageHandles[0].page, 0, memSize);

	if (pageHandles[0].page == MAP_FAILED) {
		throw new PosixException(std::string("Unable to map shared memory"), errno);
	}

	char* p1 = (char*)(pageHandles[0].page);
	pageHandles[1].page = (DoubleBufferPage*)(p1+size+sizeof(DoubleBufferPage));

	pageHandles[0].page->pagesize = size;
	pageHandles[1].page->pagesize = size;
	created=true;

	initSemaphores();
}

void DoubleBuffer::connect() {
	shmfd = shm_open(DB_MEM_ID, O_RDWR, S_IRUSR|S_IWUSR);
	if (shmfd == -1) {
		throw new PosixException(std::string("Unable to create shared memory"), errno);
	}

	pageHandles[0].page = static_cast<DoubleBufferPage*>(mmap(0, sizeof(DoubleBufferPage), PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0));

	if (ftruncate(shmfd, (pageHandles[0].page->pagesize+sizeof(DoubleBufferPage))*2)==-1) {
		throw new PosixException(std::string("Unable to set length of shared memory"), errno);
	}

	if (pageHandles[0].page == MAP_FAILED) {
		throw new PosixException(std::string("Unable to map shared memory"), errno);
	}

	created=true;

	initSemaphores();
}

void DoubleBuffer::lock(int page) {
	int r = sem_wait(pageHandles[page].sem);
	if (r == -1) {
		throw new PosixException(std::string("Unable to lock DoubleBuffer semaphore"), errno);
	}
	pageHandles[page].locked = true;
}

void DoubleBuffer::lockAny() {
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
			//std::cout << "l" << idx << std::endl;
			return;
		}
		// Try to lock the other page.
		idx = 1-idx;
	}			
}

void DoubleBuffer::lock() {
	assert(created);

	lockAny();
	return;
}

void DoubleBuffer::unlock(int page) {
	pageHandles[page].locked = false;
	int r = sem_post(pageHandles[page].sem);
	if (r == -1) {
		perror("Error unlocking: ");
	}
}
void DoubleBuffer::unlock() {
	assert(created);

	if (pageHandles[1].locked) {
		unlock(1);
		return;
	}
	if (pageHandles[0].locked) {
		unlock(0);
	}
}

void* DoubleBuffer::get() {
	assert(created);

	if (!(pageHandles[0].locked || pageHandles[1].locked))
		std::cerr << "Error: No buffer locked" << std::endl;

	return buffer->mem;
}

void DoubleBuffer::copyFrom() {
	char* src;
	char* dst;
	int other;

	if (!(pageHandles[0].locked || pageHandles[1].locked))
		std::cerr << "Error: No buffer locked" << std::endl;

	other = (buffer==pageHandles[0].page) ? 1 : 0;
	lock(other);
	src= (char*)(pageHandles[other].page->mem);
	dst= (char*)(pageHandles[1-other].page->mem);
	memcpy (dst, src, pageHandles[1-other].page->pagesize);
	unlock(other);
}

void DoubleBuffer::copyTo() {
	char* src;
	char* dst;
	int other;

	if (!(pageHandles[0].locked || pageHandles[1].locked))
		std::cerr << "Error: No buffer locked" << std::endl;

	other = (buffer==pageHandles[0].page) ? 1 : 0;
	lock(other);
	src= (char*)(pageHandles[1-other].page->mem);
	dst= (char*)(pageHandles[other].page->mem);
	memcpy (dst, src, pageHandles[1-other].page->pagesize);
	unlock(other);
}
