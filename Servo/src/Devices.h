/*
 * Devices.h
 *
 *  Created on: Jul 10, 2013
 *      Author: wilbert
 */

#ifndef DEVICES_H_
#define DEVICES_H_

class Parameter;
class SPI;
class PeriodicTimer;

class Devices {
public:
	static Devices* getInstane();
	virtual ~Devices();

	void sample();
	static void sample(void* context);

private:
	Devices();
	void sampleAngle(double frequency);
	void samplePosition(double frequency);
	void sampleBattery();

	static Devices* instance;

	SPI* spi;
	PeriodicTimer* pt;

	double angle[3];
	double angle_v[2];
	double angle_a[1];

	Parameter* par_rawAngle;
	Parameter* par_angle;
	Parameter* par_angleV;
	Parameter* par_angleA;
	Parameter* par_angleGain;
	Parameter* par_angleOffset;

	double pos[3];
	double pos_v[2];
	double pos_a[1];

	unsigned int prevRawPos;
	int encPos;

	Parameter* par_rawPos;
	Parameter* par_pos;
	Parameter* par_posV;
	Parameter* par_posA;
	Parameter* par_posGain;
	Parameter* par_posOffset;

	Parameter* par_nrIncrements;

	Parameter* par_voltage;
	Parameter* par_voltageGain;
	Parameter* par_voltageOffset;

};

#endif /* DEVICES_H_ */
