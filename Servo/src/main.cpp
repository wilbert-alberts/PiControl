#include <iostream>

#include "DoubleBuffer.h"
#include "DoubleBuffer_Imp.h"

#include "PeriodicTimer.h"

static DoubleBuffer* db;

void count(void* context);

int main(int argc, char* argv[])
{
	PeriodicTimer* pt;

	db = new DoubleBuffer_Imp();
	db->create(1024);

	pt = new PeriodicTimer(1000000);
	pt->addPeriodicFunction(count, pt);
	pt->start();
   return 0;
}

void count(void* context) {
	int ctr;
	void* p;
	PeriodicTimer* pt = static_cast<PeriodicTimer*>(context);

	db->lock();

	p = db->get();

	ctr = *(int*)p;
	ctr = ctr+1;
	*(int*)p=ctr;

	std::cout << "Operated on: " << p <<", wrote: " << ctr <<  std::endl;
	std::cout << "  margin: " << pt->getMargin()/1000 << std::endl;

	db->unlock();
}
