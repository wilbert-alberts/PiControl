/*
 * Devices.h
 *
 *  Created on: Jul 10, 2013
 *      Author: wilbert
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include <map>
#include <string>

#include "ServoModule.h"

class Parameter;
class SPI;
class PeriodicTimer;

/*
enum DeviceID {
	h1Ang, h2Ang, rawAngle, angle, angleV, angleA, angleGain, angleOffset,

	rawPos, pos, posV, posA, posGain, posOffset,

	nrIncrements,

	voltage, voltageGain, voltageOffset,

	dutyCycle, motordir, oversampling
} ;
*/

class Devices: public ServoModule {
public:
	Devices(ServoModule* wrapped);

	virtual ~Devices();

	void calculateBefore();
	void calculateAfter();

	void setSPI(SPI* s) { spi = s; }

	enum DeviceID {
		h1Ang, h2Ang, h1Ang_gain, h2Ang_gain, h1Ang_offset, h2Ang_offset,

		rawAngle, angle, angleV, angleA, angleGain, angleOffset,

		rawPos, pos, posV, posA, posGain, posOffset,

		rawGyro, gyro, gyroGain, gyroOffset,

		rawAcc, acc, accGain, accOffset,

		nrIncrements,

		voltage, voltageGain, voltageOffset,

		dutycycle, motordir, oversampling
	} ;


	double getDeviceValue(DeviceID id);
	void setDevice(DeviceID id, double val);


private:
	Parameter* createParameter(const std::string& n, double v, DeviceID id);
	void sampleAngle(double frequency);
	void samplePosition(double frequency);
	void sampleGyro(double frequency);
	void sampleAcc(double frequency);
	void sampleBattery();

	void updateDC();

	static Devices* instance;

	SPI* spi;
	std::map<DeviceID, Parameter*> devices;

	double angle_[3];
	double angle_v[2];
	double angle_a[1];

	Parameter* par_h1Ang;
	Parameter* par_h2Ang;

	Parameter* par_h1AngGain;
	Parameter* par_h2AngGain;

	Parameter* par_h1AngOffset;
	Parameter* par_h2AngOffset;

	Parameter* par_rawAngle;
	Parameter* par_angle;
	Parameter* par_angleV;
	Parameter* par_angleA;
	Parameter* par_angleGain;
	Parameter* par_angleOffset;

	double pos_[3];
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

	Parameter* par_rawGyro;
	Parameter* par_gyro;
	Parameter* par_gyroGain;
	Parameter* par_gyroOffset;

	Parameter* par_rawAcc;
	Parameter* par_acc;
	Parameter* par_accGain;
	Parameter* par_accOffset;

	Parameter* par_nrIncrements;

	Parameter* par_voltage;
	Parameter* par_voltageGain;
	Parameter* par_voltageOffset;

	Parameter* par_dutycycle;
	Parameter* par_motordir;
	Parameter* par_oversampling;
};

#endif /* DEVICES_H_ */
