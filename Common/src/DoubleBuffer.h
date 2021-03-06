/*
 * DoubleBufferImp.h
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */

#ifndef DOUBLEBUFFER_H_
#define DOUBLEBUFFER_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

struct DoubleBufferPage;

typedef struct PageHandle {
  sem_t* sem;
  bool   locked;
  DoubleBufferPage*  page;
} PageHandle ;


class DoubleBuffer {
public:
  static DoubleBuffer* getInstance();
  virtual ~DoubleBuffer();


  void create(int size);
  void connect();
  int  size();

  void lock();
  void unlock();
  void* get();

  void lockOther();
  void unlockOther();
  void* getOther();

  void copyFrom();
  void copyTo();


private:
  static DoubleBuffer* instance;

  PageHandle pageHandles[2];

  DoubleBufferPage*  buffer;
  int    shmfd;
  bool   created;

  DoubleBuffer();

  void lockAny();
  void lock(int page);
  void unlock(int page);
  void initSemaphores();
};


typedef struct DoubleBufferPage
{
  int   pagesize;
  char* mem[0];
} DoubleBufferPage;

class DoubleBufferLock {
public:
	DoubleBufferLock();
	virtual ~DoubleBufferLock();
private:
	DoubleBuffer* db;
};

#endif /* DOUBLEBUFFERIMP_H_ */
