/*
 * CmdConfigFilter.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: wilbert
 */

#include "CmdConfigFilter.h"

#include "Traces.h"
#include "Parameter.h"
#include "TimeStats.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <stdexcept>
#include <cmath>

CmdConfigFilter::CmdConfigFilter() :
		Command("cfgFilter") {
}

CmdConfigFilter::~CmdConfigFilter() {
}

void CmdConfigFilter::displayHelp() {
	std::cout << "Usage: " << getName() << " <filtername>  omega0, lambda "
			<< std::endl;
	std::cout << "\tConfigure 2'nd order filter <filtername>." << std::endl;
}

void CmdConfigFilter::execute(std::list<std::string>& args)
{
	std::stringstream converter;
	std::vector<double> a;
	std::vector<double> b;

	if (args.empty()) {
		displayHelp();
		return;
	}

	std::string name = args.front();
	args.pop_front();

	if (args.empty()) {
		displayHelp();
		return;
	}

	std::string o0 = args.front();
	args.pop_front();

	if (args.empty()) {
		displayHelp();
		return;
	}

	std::string l = args.front();
	args.pop_front();

	if (!args.empty()) {
		displayHelp();
		return;
	}

	double omega0;
	double lambda;
	double speriod = getSamplePeriod();

	converter.str(l);
	converter >> lambda;

	converter.clear();
	converter.str(o0);
	converter >> omega0;

	calculate(a,b, omega0, lambda, speriod);

	setValues(name, a, b);
}

void CmdConfigFilter::calculate(std::vector<double>& a, std::vector<double>& b, double omega0, double lambda, double speriod)
{
	double alpha;
	double beta;
	double gamma;
	double omega;

	a.resize(3);
	b.resize(3);

	omega = omega0*sqrt(1 - lambda*lambda);
	alpha = exp(-lambda*omega0*speriod);
	beta = cos(omega*speriod);
	gamma = sin(omega*speriod);

	a[0] = 1.0;
	a[1] = -2 * alpha * beta;
	a[2] = alpha*alpha;

	b[0] = 0.0;
	b[1] = 1 - alpha * (beta + gamma*lambda*omega0/omega);
	b[2] = alpha*alpha + alpha*(lambda*omega0*gamma/omega - beta);
}

double CmdConfigFilter::getSamplePeriod()
{
	Parameter* frequency= new Parameter(TimeStats::par_frequency,0.0);

	return 1.0/frequency->get();
}

void CmdConfigFilter::setValues(
		const std::string& name,
		const std::vector<double>& a,
		const std::vector<double>& b)
{
	std::stringstream ss;

	for (unsigned int i=0; i<a.size(); i++){
		ss.str("");
		ss << "Flt_" << name << "_a." << i;
		Parameter* p = new Parameter(ss.str(), 0.0);
		std::cout << "Setting " << ss.str() << " to: " << a[i] << std::endl;
		p->setDeep(a[i]);
	}
	for (unsigned int i=0; i<b.size(); i++){
		ss.str("");
		ss << "Flt_" << name << "_b." << i;
		Parameter* p = new Parameter(ss.str(), 0.0);
		std::cout << "Setting " << ss.str() << " to: " << b[i] << std::endl;
		p->setDeep(b[i]);
	}
}
