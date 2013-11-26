/*
 * Lock.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: wilbert
 */

#include "Lock.h"

#include <errno.h>


Lock::Lock()
{
	pthread_mutex_init(&mx, 0);
}

Lock::~Lock() {
	pthread_mutex_destroy(&mx);
}


void Lock::lock()
{
	int r = pthread_mutex_lock(&mx);
	if (r!=0)
		throw std::system_error(errno, std::system_category(),"unable to lock");
}


bool Lock::tryLock()
{
	int r = pthread_mutex_trylock(&mx);
	if ((r!=0) && (r != EBUSY))
		throw std::system_error(errno, std::system_category(),"unable trying to lock");
	return r == 0;
}


void Lock::unlock()
{
	int r = pthread_mutex_unlock(&mx);
	if (r!=0)
		throw std::system_error(errno, std::system_category(),"unable to lock");
}

