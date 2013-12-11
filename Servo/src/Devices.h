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

class Devices {
public:
	static Devices* getInstance();
	virtual ~Devices();

	void sample();
	static void sample(void* context);
	void update();
	static void update(void* context);

	enum DeviceID {
		h1Ang, h2Ang, rawAngle, angle, angleV, angleA, angleGain, angleOffset,

		rawPos, pos, posV, posA, posGain, posOffset,

		rawGyro, gyro, gyroGain, gyroOffset,

		nrIncrements,

		voltage, voltageGain, voltageOffset,

		dutycycle, motordir, oversampling
	} ;


	double getDeviceValue(DeviceID id);
	void setDevice(DeviceID id, double val);


private:
	Devices();
	Parameter* createParameter(const std::string& n, double v, DeviceID id);
	void sampleAngle(double frequency);
	void samplePosition(double frequency);
	void sampleGyro(double frequency);
	void sampleBattery();

	void updateDC();

	static Devices* instance;

	SPI* spi;
	PeriodicTimer* pt;

	std::map<DeviceID, Parameter*> devices;

	double angle_[3];
	double angle_v[2];
	double angle_a[1];

	Parameter* par_h1Ang;
	Parameter* par_h2Ang;
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

	Parameter* par_nrIncrements;

	Parameter* par_voltage;
	Parameter* par_voltageGain;
	Parameter* par_voltageOffset;

	Parameter* par_dutycycle;
	Parameter* par_motordir;
	Parameter* par_oversampling;
};

#endif /* DEVICES_H_ */
