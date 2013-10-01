/*
 * TimeStats.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wilbert
 */

#ifndef TIMESTATS_H_
#define TIMESTATS_H_

#include <string>

class TimeStats {
public:
	static const std::string par_sampleCounter;
	static const std::string par_minMargin;
	static const std::string par_maxMargin;
	static const std::string par_margin;
	static const std::string par_overruns;
	static const std::string par_reset;
	static const std::string par_frequency;
};

#endif /* TIMESTATS_H_ */
