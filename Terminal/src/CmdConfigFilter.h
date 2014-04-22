/*
 * CmdDumpTraces.h
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#ifndef CmdConfigFilter_H_
#define CmdConfigFilter_H_

#include "Command.h"

#include <vector>
#include <string>

class CmdConfigFilter : Command{
public:
	CmdConfigFilter();
	virtual ~CmdConfigFilter();
	virtual void displayHelp(std::ostream& out);
	virtual void execute(std::ostream& out);

private:
	void calculate(
			std::vector<double>& a,
			std::vector<double>& b,
			double omega0,
			double lambda,
			double speriod);
	double getSamplePeriod();
	void   setValues(
			const std::string& name,
			const std::vector<double>& a,
			const std::vector<double>& b);

};

#endif /* CmdConfigFilter_H_ */
