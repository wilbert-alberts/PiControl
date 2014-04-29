/*
 * Controller.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: wilbert
 */

#include <assert.h>
#include <iostream>

#include "Controller.h"
#include "Motor.h"
#include "Devices.h"
#include "Parameter.h"
#include "Filter.h"
#include "PeriodicTimer.h"

Controller::Controller(ServoModule* pre)
: ServoModule("Controller", pre)
, motor(0)
, devs(0)
, prevPosError(0.0)
, prevAngError(0.0)
, relPosOffset(0.0)
, sumError(0.0)
{
	enabled = createParameter("enabled", 0);
	pos_sp = createParameter("pos_sp", 0);
	pos_kp = createParameter("pos_kp", 0);
	pos_kd = createParameter("pos_kd", 0);
	pos_ki = createParameter("pos_ki", 0);

	ang_sp_kp = createParameter("ang_sp_kp", 0);

	ang_sp = createParameter("ang_sp", 0);
	ang_kp = createParameter("ang_kp", 0);
	ang_kd = createParameter("ang_kd", 0);
	ang_ki = createParameter("ang_ki", 0);

	ang_mix = new Parameter("Controller.ang_mix", 0);
	ang_mix_d1 = new Parameter("Controller.ang_mix_d1", 0);
	ang_mix_d2 = new Parameter("Controller.ang_mix_d2", 0);

	angErrorParam = createParameter("angError", 0);
	posErrorParam = createParameter("posError", 0);

	co_poskp = createParameter("co_poskp", 0);
	co_poskd = createParameter("co_poskd", 0);
	co_poski = createParameter("co_poski", 0);
	co_angkp = createParameter("co_angkp", 0);
	co_angkd = createParameter("co_angkd", 0);

	vang_flt = createParameter("angV_flt", 0);
	ang_flt  = createParameter("ang_flt", 0);

	vang_raw = createParameter("angV_raw", 0);
	ang_raw  = createParameter("ang_raw", 0);

	accAng_raw = new Parameter("Controller.accAng_raw",0);
	accAng_flt = new Parameter("Controller.accAng_flt",0);

	pos_flt  = new Parameter("Controller.pos_flt", 0);
	pos_raw  = new Parameter("Controller.pos_raw", 0);

	mmdcMinAng = new Parameter("Controller.minAng", -100);
	mmdcMaxAng = new Parameter("Controller.maxAng",  100);

	injAmpl = createParameter("inj_ampl", 0.0);
	injFreq = createParameter("inj_freq", 0.5);
	noiseSample = createParameter("noise", 0.0);

	flt_pos = new Filter("pos", 3);
	flt_ang = new Filter("ang", 3);
	flt_vang = new Filter("gyro", 3);
	flt_vang_hpf = new HPFilter("gyro_hpf", 3);
	flt_acc = new Filter("acc", 3);

	updateActualPosition = true;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::calculateAfter() {
	assert(devs!=0);
	assert(motor!=0);

	if (mmdcSafe())
	{
		calculateModel();
	}
	else
	{
		disableController();
	}
}

bool Controller::mmdcSafe()
{
	if (*enabled==0.0)
		return true;

	//double ang = devs->getDeviceValue(Devices::angle);
	double ang = *ang_mix;

	return ((ang >= *mmdcMinAng) &&
	        (ang <= *mmdcMaxAng));
}

void Controller::disableController()
{
	motor->setTorque(0.0);
	*enabled = 0.0;
	updateActualPosition = true;
}

void Controller::calculateModel()
{
	double tq(0.0);
	double pos(0.0);
	double posError(0.0);
	double posVError(0.0);
	double angError(0.0);
	double angVError(0.0);

	double ang(0.0);
	double angV1(0.0);
	double angV2(0.0);
	double angV3(0.0);

	static double am_d1(0.0);
	static double am_d2(0.0);
	static double am(0.0);
	static double am_i(0.0);


	// Get position from device.
	pos = *devs->getDevice(Devices::ENCPOS);
	*pos_raw = pos;
	pos = filterDevice(flt_pos, pos);
	*pos_flt = pos;

	if (updateActualPosition) {
		// Get actual position from Devices
		sumError = 0;
		prevPosError = 0;
		prevAngError = 0;
		relPosOffset = pos;
		updateActualPosition = false;
		am_i=0.0;
	}

	// Determine position error
	posError = *pos_sp - (pos - relPosOffset);
	posVError = (posError - prevPosError);

	// Determine setpoint for angle.
	*ang_sp = posError * *ang_sp_kp;

	// Limit angle setpoint between -1 and 1 degree
	if (*ang_sp <-1) *ang_sp = -1;
	if (*ang_sp > 1) *ang_sp =  1;

	// Get angle from height sensor
//	ang = devs->getDeviceValue(Devices::angle);
//	ang_raw->set(ang);
//	ang = filterDevice(flt_ang, ang);
//	ang_flt->set(ang);

	// Get angle from Device
	ang = *devs->getDevice(Devices::HEIGHT);
	*ang_raw = ang;
	//ang = filterDevice(flt_ang, ang);
	*ang_flt = ang;

	// Determine angle error
	angError = *ang_sp - ang;
	angVError = (angError - prevAngError);

	// Get angular velocity from gyro Device
	angV1 = *devs->getDevice(Devices::GYRO);
	*vang_raw = angV1;
	angV2 = filterDevice(flt_vang_hpf, angV1);
	angV3 = filterDevice(flt_vang, angV2);
	*vang_flt = angV3;

	// Determine mixed angle
	am_d1 = am_d1 + angV2 / 100.0;
	am_d2 = ang;
	am = 0.98*(am + angV2 / 100.0) + 0.02*am_d2;
	am_i = am_i + am/100.0;

	*ang_mix_d1 = am_d1 ;
	*ang_mix_d2 = am_d2 ;
	*ang_mix    = am ;


	// Determine integrated position error
	sumError += posError;
	if ((posError > 0 && prevPosError < 0)
			|| (posError < 0 && prevPosError > 0))
		sumError = 0;


	// Store individual controller contributions
	*co_poskp = *pos_kp * posError;
	*co_poskd = *pos_kd * posVError;
	*co_poski = *pos_ki * sumError;

	*co_angkp = *ang_kp * (*ang_mix);
	*co_angkd = *ang_kd * angV3;

	// Calculate final torque
	tq = (pos_kp->get() * posError +
          pos_kd->get() * posVError +
		  pos_ki->get() * sumError +
		  ang_kp->get() * am +
		  ang_kd->get() * angV3 +
		  ang_ki->get() * am_i);

	// Inject noise
	//tq = doInject(tq);

	// Perform safety check
	if (!mmdcSafe())
	{
		disableController();
		return;
	}


	// Set torque.
	if (*enabled != 0.0) {
		motor->setTorque(tq);

		prevPosError = posError;
		prevAngError = angError;

	} else {
		//motor->setTorque(0.0);
		updateActualPosition = true;
	}

	*posErrorParam = posError;
	*angErrorParam = angError;

}


double Controller::doInject(double t) {
	double sample = ndis(generator);

	*noiseSample = sample;

	if (sample < *injFreq) return t - *injAmpl;
	if (sample > *injFreq) return t + *injAmpl;
	return t;
}


double Controller::filterDevice(Filter* f, double i)
{
	return f->calculate(i);
}

double Controller::filterDevice(HPFilter* f, double i)
{
	return f->calculate(i);
}
