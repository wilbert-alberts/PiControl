/*
 * main.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: wilbert
 */


#include <iostream>

#include "DoubleBuffer.h"
#include "DoubleBuffer_Imp.h"

#include "Parameter.h"


int main(int argc, char* argv[])
{
	DoubleBuffer* db = new DoubleBuffer_Imp();
	Parameter* par;

	db->connect();

	db->lock();

	int nrParams = Parameter::getNrParameters(db);
	std::cout << "Numnber of parameter: " << nrParams << std::endl;

	par = new Parameter(db, 0);
	std::cout << "Parameter 0,  " << par->getName() << ": " <<  par->get() << std::endl;

 	db->unlock();

	return 0;
}
