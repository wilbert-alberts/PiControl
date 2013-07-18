/*
 * Parameter.h
 *
 *  Created on: Jun 15, 2013
 *      Author: wilbert
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>

class DoubleBuffer;

struct DB_mem;

class Parameter {
public:
  Parameter(const std::string& name, double value = 0.0);
  Parameter(int idx);

  virtual ~Parameter();

  std::string getName();
  double get();
  void set(double v);
  void setDeep(double v); // Carefull: requires locking both pages

  static int getNrParameters();
  static int findParameter(const std::string& name);

  //static void setByIdx(int i, double v);
  static double getByIdx(int i);
  static std::string getNameByIdx(int i);

private:
  DoubleBuffer* db;
  int idx;

  void createParameter(DB_mem* mem, const std::string& name, double value);
  static int findParameterInMem(DB_mem* mem, const std::string& name);

};
#endif /* PARAMETER_H_ */
