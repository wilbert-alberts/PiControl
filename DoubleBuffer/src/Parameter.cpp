/*
 * Parameter.cpp
 *
 *  Created on: Jun 15, 2013
 *      Author: wilbert
 */

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr int MAXNAMESIZE = 64;

const std::string Parameter::dumpAllParametersCommand("dumpParameters");

typedef struct {
  double value;
  char   name[MAXNAMESIZE];
} DB_Parameter;

typedef struct {
  int nrParameters;
  DB_Parameter params[];
} DB_mem;


Parameter::Parameter(const std::string& name)
: db(DoubleBuffer::getInstance())
{
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());

  idx = findParameter(name);
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

void Parameter::setByIdx(int i, double v) {
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());

  assert(i<p->nrParameters);

  DB_Parameter* par = &p->params[i];

  par->value = v;
}

int Parameter::getNrParameters() {
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());
  return p->nrParameters;
}

int Parameter::findParameter(const std::string& name)
{
  DB_mem* p=static_cast<DB_mem*>(DoubleBuffer::getInstance()->get());
  for (int i=0; i<p->nrParameters; i++) {
    if (strncmp(name.c_str(), p->params[i].name,MAXNAMESIZE-1)==0) {
      return i;
    }
  }
  return  -1;
}

void Parameter::execDumpAllParameters(int argc, char** /*argv[]*/)
{
  if (argc!=1) {
    throw std::runtime_error("Usage: " + dumpAllParametersCommand );
  }

  for (int i=0; i<Parameter::getNrParameters(); i++) {
    std::string parname = Parameter::getNameByIdx(i);
    double value = Parameter::getByIdx(i);
    std::cout << parname << "\t" << value << std::endl;
  }
}
