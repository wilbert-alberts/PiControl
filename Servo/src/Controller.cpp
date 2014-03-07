/*
 * Controller.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: wilbert
 */

#include <iostream>

#include "Controller.h"
#include "Motor.h"
#include "Devices.h"
#include "Parameter.h"
#include "Filter.h"


Controller* Controller::getInstance() {
	static Controller* instance(0);

	if (instance == 0)
		instance = new Controller();
	return instance;
}

Controller::Controller()
: ServoModule("Controller" , 0)
, prevPosError(0.0)
, prevAngError(0.0)
, relPosOffset(0.0)
, sumError(0.0)
{
	enabled = createParameter("enabled");
	pos_sp = createParameter("pos_sp");
	pos_kp = createParameter("pos_kp");
	pos_kd = createParameter("pos_kd");
	pos_ki = createParameter("pos_ki");

	ang_sp_kp = createParameter("ang_sp_kp");

	ang_sp = createParameter("ang_sp");
	ang_kp = createParameter("ang_kp");
	ang_kd = createParameter("ang_kd");
	ang_ki = createParameter("ang_ki");


	angErrorParam = createParameter("angError");
	posErrorParam = createParameter("posError");

	co_poskp = createParameter("co_poskp");
	co_poskd = createParameter("co_poskd");
	co_poski = createParameter("co_poski");
	co_angkp = createParameter("co_angkp");
	co_angkd = createParameter("co_angkd");

	vang_flt = createParameter("angV_flt");
	ang_flt  = createParameter("ang_flt");

	vang_raw = createParameter("angV_raw");
	ang_raw  = createParameter("ang_raw");

	pos_flt  = createParameter("pos_flt");
	pos_raw  = createParameter("pos_raw");

	mmdcMinAng = createParameter("minAng", -100);
	mmdcMaxAng = createParameter("maxAng",  100);

	injAmpl = createParameter("inj_ampl");
	injFreq = createParameter("inj_freq", 0.5);
	noiseSample = createParameter("noise");

	flt_pos = new Filter("pos", 3);
	flt_ang = new Filter("ang", 3);
	flt_vang = new Filter("gyro", 3);
	flt_vang_hpf = new HPFilter("gyro_hpf", 3);

	motor = Motor::getInstance();
	devs = Devices::getInstance();
	updateActualPosition = true;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::sample() {
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

	double ang = devs->getDeviceValue(Devices::angle);

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

	// Get position from device.
	pos = devs->getDeviceValue(Devices::pos);
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
	}

	// Determine position error
	posError = *pos_sp - (pos - relPosOffset);
	posVError = (posError - prevPosError);

	// Determine setpoint for angle.
	*ang_sp = posError * *ang_sp_kp;

	// Limit angle setpoint between -1 and 1 degree
	if (*ang_sp <-1) *ang_sp = -1;
	if (*ang_sp > 1) *ang_sp =  1;

	// Get angle from Device
	ang = devs->getDeviceValue(Devices::angle);
	*ang_raw = ang;
	ang = filterDevice(flt_ang, ang);
	*ang_flt = ang;

	// Determine angle error
	angError = *ang_sp - ang;
	angVError = (angError - prevAngError);

	// Get angular velocity from gyro Device
	angV1 = devs->getDeviceValue(Devices::gyro);
	*vang_raw = angV1;
	angV2 = filterDevice(flt_vang_hpf, angV1);
	angV3 = filterDevice(flt_vang, angV2);
	*vang_flt = angV3;

	// Determine integrated position error
	sumError += posError;
	if ((posError > 0 && prevPosError < 0)
			|| (posError < 0 && prevPosError > 0))
		sumError = 0;


	// Store individual controller contributions
	*co_poskp = *pos_kp * posError;
	*co_poskd = *pos_kd * posVError;
	*co_poski = *pos_ki * sumError;

	*co_angkp = *ang_kp * angError;
	*co_angkd = *ang_kd * angV3;

	// Calculate final torque
	tq = (*pos_kp * posError +
          *pos_kd * posVError +
		  *pos_ki * sumError +
		  *ang_kp * angError +
		  *ang_kd * angV3);


	// Inject noise
	//tq = doInject(tq);

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

