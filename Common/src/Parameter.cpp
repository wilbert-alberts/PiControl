/*
 * Parameter.cpp
 *
 *  Created on: Jun 15, 2013
 *      Author: wilbert
 */

#include "Parameter.h"
#include "DoubleBuffer.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr int MaxNameLength = 64;

typedef struct {
  double value;
  char   name[MaxNameLength];
} DB_Parameter;

typedef struct DB_mem{
  int nrParameters;
  DB_Parameter params[];
} DB_mem;


Parameter::Parameter(const std::string& name, double value)
: db(DoubleBuffer::getInstance())
{
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());

  createParameter(p, name, value);

  DoubleBuffer::getInstance()->lockOther();
  DB_mem* o=static_cast<DB_mem*>(DoubleBuffer::getInstance()->getOther());
  createParameter(o, name, value);
  DoubleBuffer::getInstance()->unlockOther();
}

void Parameter::createParameter(DB_mem* p, const std::string& name, double value)
{
	  int memSize = DoubleBuffer::getInstance()->size();
	  int maxNrParameters = (memSize - sizeof(DB_mem))/sizeof(DB_Parameter);
	  if (p->nrParameters == maxNrParameters)
		  throw std::runtime_error("Exceeding memory space while creating Parameter: " + name);


	  idx = findParameterInMem(p, name);
	  if (idx<0) {
	    // Create parameter in double buffer.
	    idx = p->nrParameters;
	    p->nrParameters++;
	    DB_Parameter* par = (p->params)+(idx);
	    if (name.length() > MaxNameLength-1) {
	    	std::clog << "Warning: only using " << MaxNameLength-1 << " characters from: " << name << std::endl;
	    }
	    strncpy(par->name, name.c_str(), MaxNameLength-1);
	    par->value = value;
	  }
}

Parameter::Parameter(int i)
: db(DoubleBuffer::getInstance()), idx(i)
{

}

Parameter::~Parameter() {

}

int Parameter::getIndex() {
	return idx;
}

std::string Parameter::getName() {
  DB_mem* p=static_cast<DB_mem*>(db->get());
  DB_Parameter* par = &p->params[idx];
  std::string result;

  result.assign(par->name);
  return result;
}

std::string Parameter::getNameByIdx(int i) {
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());
  DB_Parameter* par = &p->params[i];
  return std::string(par->name);
}

double Parameter::get() {
  DB_mem* p=static_cast<DB_mem*>(db->get());
  DB_Parameter* par = &p->params[idx];

  return par->value;
}

double Parameter::getByIdx(int i) {
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());

  assert(i<p->nrParameters);

  DB_Parameter* par = &p->params[i];

  return par->value;
}

void Parameter::set(double v) {
  DB_mem* p=static_cast<DB_mem*>(db->get());
  DB_Parameter* par = &p->params[idx];

  par->value = v;
}

void Parameter::setDeep(double v) {
	  DB_mem* p=static_cast<DB_mem*>(db->get());
	  DB_Parameter* par = &p->params[idx];
	  par->value = v;

	  db->lockOther();
	  DB_mem* o=static_cast<DB_mem*>(db->getOther());
	  DB_Parameter* opar = &o->params[idx];
	  opar->value = v;
	  db->unlockOther();
}

/*
void Parameter::setByIdx(int i, double v) {
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());

  assert(i<p->nrParameters);

  DB_Parameter* par = &p->params[i];

  par->value = v;
}
*/

int Parameter::getNrParameters() {
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());
  return p->nrParameters;
}

int Parameter::findParameter(const std::string& name)
{
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());
  return findParameterInMem(p, name);
}

int Parameter::findParameterInMem(DB_mem* p, const std::string& name)
{
  for (int i=0; i<p->nrParameters; i++) {
    if (strncmp(name.c_str(), p->params[i].name,MaxNameLength-1)==0) {
      return i;
    }
  }
  return  -1;
}
