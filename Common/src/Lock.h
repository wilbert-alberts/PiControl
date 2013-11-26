/*
 * Lock.h
 *
 *  Created on: Nov 25, 2013
 *      Author: wilbert
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <pthread.h>

class Lock
{
public:
	Lock();
	virtual ~Lock();

	void lock();
	bool tryLock();
	void unlock();

private:
	pthread_mutex_t mx;

};

#endif /* LOCK_H_ */
