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
#include "PeriodicTimer.h"

Controller* Controller::instance = 0;

Controller* Controller::getInstance() {
	if (instance == 0)
		instance = new Controller();
	return instance;
}

Controller::Controller()
: prevPosError(0.0)
, prevAngError(0.0)
, relPosOffset(0.0)
, sumError(0.0)
{
	enabled = new Parameter("Controller.enabled", 0);
	pos_sp = new Parameter("Controller.pos_sp", 0);
	pos_kp = new Parameter("Controller.pos_kp", 0);
	pos_kd = new Parameter("Controller.pos_kd", 0);
	pos_ki = new Parameter("Controller.pos_ki", 0);

	ang_sp_kp = new Parameter("Controller.ang_sp_kp", 0);

	ang_sp = new Parameter("Controller.ang_sp", 0);
	ang_kp = new Parameter("Controller.ang_kp", 0);
	ang_kd = new Parameter("Controller.ang_kd", 0);
	ang_ki = new Parameter("Controller.ang_ki", 0);

	ang_mix = new Parameter("Controller.ang_mix", 0);
	ang_mix_d1 = new Parameter("Controller.ang_mix_d1", 0);
	ang_mix_d2 = new Parameter("Controller.ang_mix_d2", 0);

	angErrorParam = new Parameter("Controller.angError", 0);
	posErrorParam = new Parameter("Controller.posError", 0);

	co_poskp = new Parameter("Controller.co_poskp", 0);
	co_poskd = new Parameter("Controller.co_poskd", 0);
	co_poski = new Parameter("Controller.co_poski", 0);
	co_angkp = new Parameter("Controller.co_angkp", 0);
	co_angkd = new Parameter("Controller.co_angkd", 0);

	vang_flt = new Parameter("Controller.angV_flt", 0);
	ang_flt  = new Parameter("Controller.ang_flt", 0);

	vang_raw = new Parameter("Controller.angV_raw", 0);
	ang_raw  = new Parameter("Controller.ang_raw", 0);

	accAng_raw = new Parameter("Controller.accAng_raw",0);
	accAng_flt = new Parameter("Controller.accAng_flt",0);

	pos_flt  = new Parameter("Controller.pos_flt", 0);
	pos_raw  = new Parameter("Controller.pos_raw", 0);

	mmdcMinAng = new Parameter("Controller.minAng", -100);
	mmdcMaxAng = new Parameter("Controller.maxAng",  100);

	injAmpl = new Parameter("Motor.inj_ampl", 0.0);
	injFreq = new Parameter("Motor.inj_freq", 0.5);
	noiseSample = new Parameter("Motor.noise", 0.0);

	flt_pos = new Filter("pos", 3);
	flt_ang = new Filter("ang", 3);
	flt_vang = new Filter("gyro", 3);
	flt_vang_hpf = new HPFilter("gyro_hpf", 3);
	flt_acc = new Filter("acc", 3);

	motor = Motor::getInstance();
	devs = Devices::getInstance();
	updateActualPosition = true;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::sample() {
	calculateModel();
}

bool Controller::mmdcSafe()
{
	if (enabled->get()==0.0)
		return true;

	//double ang = devs->getDeviceValue(Devices::angle);
	double ang = ang_mix->get();

	return ((ang >= mmdcMinAng->get()) &&
	        (ang <= mmdcMaxAng->get()));
}

void Controller::disableController()
{
	motor->setTorque(0.0);
	enabled->set(0.0);
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
	pos = devs->getDeviceValue(Devices::pos);
	pos_raw->set(pos);
	pos = filterDevice(flt_pos, pos);
	pos_flt->set(pos);

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
	posError = pos_sp->get() - (pos - relPosOffset);
	posVError = (posError - prevPosError);

	// Determine setpoint for angle.
	ang_sp->set(posError * ang_sp_kp->get());

	// Limit angle setpoint between -1 and 1 degree
	if (ang_sp->get() <-1) ang_sp->set(-1);
	if (ang_sp->get() > 1) ang_sp->set( 1);

	// Get angle from height sensor
//	ang = devs->getDeviceValue(Devices::angle);
//	ang_raw->set(ang);
//	ang = filterDevice(flt_ang, ang);
//	ang_flt->set(ang);

	// Get angle from accelerometer
	ang = devs->getDeviceValue(Devices::acc);
	accAng_raw->set(ang);
	//ang = filterDevice(flt_acc, ang);
	accAng_flt->set(ang);

	// Determine angle error
	angError = ang_sp->get() - ang;
	angVError = (angError - prevAngError);

	// Get angular velocity from gyro Device
	angV1 = devs->getDeviceValue(Devices::gyro);
	vang_raw->set(angV1);
	angV2 = filterDevice(flt_vang_hpf, angV1);
	angV3 = filterDevice(flt_vang, angV2);
	vang_flt->set(angV3);

	// Determine mixed angle
	am_d1 = am_d1 + angV2 / 100.0;
	am_d2 = ang;
	am = 0.98*(am + angV2 / 100.0) + 0.02*am_d2;
	am_i = am_i + am/100.0;

	ang_mix_d1->set(am_d1);
	ang_mix_d2->set(am_d2);
	ang_mix->set(am);


	// Determine integrated position error
	sumError += posError;
	if ((posError > 0 && prevPosError < 0)
			|| (posError < 0 && prevPosError > 0))
		sumError = 0;


	// Store individual controller contributions
	co_poskp->set(pos_kp->get() * posError);
	co_poskd->set(pos_kd->get() * posVError);
	co_poski->set(pos_ki->get() * sumError);

	co_angkp->set(ang_kp->get() * ang_mix->get());
	co_angkd->set(ang_kd->get() * angV3);

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
	if (enabled->get() != 0.0) {
		motor->setTorque(tq);

		prevPosError = posError;
		prevAngError = angError;

	} else {
		//motor->setTorque(0.0);
		updateActualPosition = true;
	}

	posErrorParam->set(posError);
	angErrorParam->set(angError);

}


double Controller::doInject(double t) {
	double sample = ndis(generator);

	noiseSample->set(sample);

	if (sample < injFreq->get()) return t - injAmpl->get();
	if (sample > injFreq->get()) return t + injAmpl->get();
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

void Controller::sample(void* /* context */) {
	Controller* me = Controller::getInstance(); // static_cast<Controller*>(context);
	me->sample();
}
