/*
 * Controller.h
 *
 *  Created on: Oct 4, 2013
 *      Author: wilbert
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class Parameter;
class Motor;
class Devices;
class Filter;
class HPFilter;

#include <list>
#include <random>

#include "ServoModule.h"

class Controller : public ServoModule {
public:
	Controller(ServoModule* pre);
	virtual ~Controller();

	void calculateAfter();

	void setDevices(Devices* devs) {
		this->devs = devs;
	}

	void setMotor(Motor* motor) {
		this->motor = motor;
	}

private:
	void calculateModel();
	bool mmdcSafe();
	void disableController();
	double filterDevice(Filter* f, double i);
	double filterDevice(HPFilter* f, double i);
	double doInject(double t);

	static Controller* instance;

	Parameter* enabled;
	Parameter* pos_sp;
	Parameter* pos_kp;
	Parameter* pos_kd;
	Parameter* pos_ki;
	Parameter* ang_sp_kp;
	Parameter* ang_sp;
	Parameter* ang_kp;
	Parameter* ang_kd;
	Parameter* ang_ki;
	Parameter* posErrorParam;
	Parameter* angErrorParam;
	Parameter* mmdcMinAng;
	Parameter* mmdcMaxAng;

	Parameter* co_poskp;
	Parameter* co_poskd;
	Parameter* co_poski;
	Parameter* co_angkp;
	Parameter* co_angkd;

	Parameter* pos_raw;
	Parameter* pos_flt;

	Parameter* ang_flt;
	Parameter* ang_raw;
	Parameter* vang_flt;
	Parameter* vang_raw;


	Filter* flt_pos;
	Filter* flt_ang;
	Filter* flt_vang;
	HPFilter* flt_vang_hpf;

	std::default_random_engine generator;
	std::normal_distribution<double> ndis;

	Parameter* injAmpl;
	Parameter* injFreq;
	Parameter* noiseSample;

	Motor* motor;
	Devices* devs;

	bool updateActualPosition;

	double prevPosError;
	double prevAngError;
	double relPosOffset;
	double sumError;

};

#endif /* CONTROLLER_H_ */
