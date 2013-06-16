/*
 * ParamStore.cpp
 *
 *  Created on: Jun 15, 2013
 *      Author: wilbert
 */

#include <string.h>
#include <iostream>
#include <stdlib.h>


#include "ParamStore.h"
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

ParamStore::ParamStore(DoubleBuffer* _db)
:db(_db)
{
	// TODO Auto-generated constructor stub

}

ParamStore::~ParamStore() {
	// TODO Auto-generated destructor stub
}

Parameter* ParamStore::createParameter(const std::string& name)
{
	DB_mem* p=static_cast<DB_mem*>(db->get());

	int parIdx = p->nrParameters;

	DB_Parameter* par = (p->params)+(parIdx);

	strncpy(par->name, name.c_str(), MAXNAMESIZE-1);
	par->value = 0.0;

	return new Parameter(db, parIdx);
}

int ParamStore::getNrParameters() {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	return p->nrParameters;
}

Parameter* ParamStore::getParameter(int i)
{
	DB_mem* p=static_cast<DB_mem*>(db->get());
	if (i<p->nrParameters)
		return new Parameter(db, i);
	else {
		std::cerr << "Trying to retrieve Parameter by illegal index" <<  std::endl;
		exit(-1);
	}
	return 0;
}

Parameter::Parameter(DoubleBuffer* _db, int _i)
: db(_db), idx(_i)
{

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
