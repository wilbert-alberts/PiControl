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
#include <assert.h>

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

	idx = findParameter(_db, name);
	if (idx<0) {
		// Create parameter in double buffer.
		idx = p->nrParameters;
		p->nrParameters++;
		DB_Parameter* par = (p->params)+(idx);
		strncpy(par->name, name.c_str(), MAXNAMESIZE-1);
		par->value = 0.0;
	}
}

Parameter::~Parameter() {

}

std::string Parameter::getName() {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[idx];
	std::string result;

	result.assign(par->name);
	return result;
}

std::string Parameter::getNameByIdx(DoubleBuffer* db, int i) {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[i];
	return std::string(par->name);
}

double Parameter::get() {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[idx];

	return par->value;
}

double Parameter::getByIdx(DoubleBuffer* db, int i) {
	DB_mem* p=static_cast<DB_mem*>(db->get());

	assert(i<p->nrParameters);

	DB_Parameter* par = &p->params[i];

	return par->value;
}

void Parameter::set(double v) {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	DB_Parameter* par = &p->params[idx];

	par->value = v;
}

void Parameter::setByIdx(DoubleBuffer* db, int i, double v) {
	DB_mem* p=static_cast<DB_mem*>(db->get());

	assert(i<p->nrParameters);

	DB_Parameter* par = &p->params[i];

	par->value = v;
}

int Parameter::getNrParameters(DoubleBuffer* db) {
	DB_mem* p=static_cast<DB_mem*>(db->get());
	return p->nrParameters;
}

int Parameter::findParameter(DoubleBuffer* db, const std::string& name) 
{
	DB_mem* p=static_cast<DB_mem*>(db->get());
	for (int i=0; i<p->nrParameters; i++) {
		if (strncmp(name.c_str(), p->params[i].name,MAXNAMESIZE-1)==0) {
			return i;
		}
	}
	return  -1;
}

