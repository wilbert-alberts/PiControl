/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */


#include <iostream>

#include "DoubleBuffer.h"
#include "DoubleBuffer_Imp.h"


int main(int argc, char* argv[])
{
	DoubleBuffer* db = new DoubleBuffer_Imp();

	db->connect();

	db->lock();

	void* p = db->get();
	int *i = static_cast<int*>(p);

	std::cout << "Reading: " << *i << std::endl;

	db->unlock();

	return 0;
}
