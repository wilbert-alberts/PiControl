/*
 * Filter.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: ben
 */

#include <sstream>

#include "Filter.h"
#include "Parameter.h"



Filter::Filter(const std::string& n, int d)
: id(n)
, dimension(d)
{
	par_a = new Parameter*[d];
	par_b = new Parameter*[d];

	for (int i=0; i<d; i++) {
		std::ostringstream an;
		std::ostringstream bn;

		an << "Flt_" << n << "_a." << i;
		bn << "Flt_" << n << "_b." << i;

		par_a[i] = new Parameter(an.str(), 0.0);
		par_b[i] = new Parameter(bn.str(), 0.0);
	}

	for (int i=0; i<d; i++) {
		in.push_front(0.0);
		out.push_front(0.0);
	}
}

Filter::~Filter() {
	delete[] par_a;
	delete[] par_b;
}

double Filter::calculate(double i)
{
	double B;
	double A;
	double o;

	// Note: newest input at index 0
	//       oldest input at index d
	in.push_front(i);
	in.pop_back();
	for (int i=0; i<dimension; i++) {
		// B += b[i]*in[i]
		B += par_b[i]->get()*in.at(i);
	}

	for (int i=1; i<dimension; i++) {
		A += par_a[i]->get()*out.at(i-1);
	}

	o = B-A;

	out.push_front(o);
	out.pop_back();

	return o;
}

