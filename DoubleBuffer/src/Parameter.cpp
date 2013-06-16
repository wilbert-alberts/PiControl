/*
 * Parameter.cpp
 *
 *  Created on: Jun 15, 2013
 *      Author: wilbert
 */

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "Parameter.h"
#include "DoubleBuffer.h"

#define MAXNAMESIZE (64)

typedef struct {
	double value;
	char   name[MAXNAMESIZE];
} DB_Parameter;

typedef struct {
	int nrParameters;
	DB_Parameter params[];
} DB_mem;


Parameter::Parameter(DoubleBuffer* _db, const std::string& name)
: db(_db)
{
	DB_mem* p=static_cast<DB_mem*>(db->get());

	idx = p->nrParameters;

	DB_Parameter* par = (p->params)+(idx);

	strncpy(par->name, name.c_str(), MAXNAMESIZE-1);
	par->value = 0.0;

	p->nrParameters++;
}

Parameter::Parameter(DoubleBuffer* _db, int i)
: db(_db),idx(i)
{
	DB_mem* p=static_cast<DB_mem*>(db->get());
	if (i>=p->nrParameters) {
		std::cerr << "Trying to retrieve Parameter by illegal index" <<  std::endl;
		exit(-1);
	}
}


Parameter::~Parameter() {

}

void Parameter::getName(std::string& n) {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[idx];
	std::string result;

	result.assign(par->name);
}

std::string Parameter::getName() {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[idx];
	std::string result;

	result.assign(par->name);
	return result;
}

double Parameter::get() {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[idx];

	return par->value;
}

void Parameter::set(double v) {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[idx];

	par->value = v;
}

int Parameter::getNrParameters(DoubleBuffer* db) {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	return p->nrParameters;
}

