/*
 * Controller.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: wilbert
 */

#include "Controller.h"
#include "Motor.h"
#include "Devices.h"
#include "Parameter.h"
#include "Filter.h"


Controller* Controller::instance = 0;

Controller* Controller::getInstance() {
	if (instance == 0)
		instance = new Controller();
	return instance;
}

Controller::Controller() {
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


	angErrorParam = new Parameter("Controller.angError", 0);
	posErrorParam = new Parameter("Controller.posError", 0);

	co_poskp = new Parameter("Controller.co_poskp", 0);
	co_poskd = new Parameter("Controller.co_poskd", 0);
	co_poski = new Parameter("Controller.co_poski", 0);
	co_angkp = new Parameter("Controller.co_angkp", 0);
	co_angkd = new Parameter("Controller.co_angkd", 0);

	vang_flt = new Parameter("Controller.angV_flt", 0);
	ang_flt  = new Parameter("Controller.ang_flt", 0);

	mmdcMinAng = new Parameter("Controller.minAng", -100);
	mmdcMaxAng = new Parameter("Controller.maxAng",  100);

	injAmpl = new Parameter("Motor.inj_ampl", 0.0);
	injFreq = new Parameter("Motor.inj_freq", 0.5);
	noiseSample = new Parameter("Motor.noise", 0.0);

	flt_pos = new Filter("pos", 3);
	flt_ang = new Filter("ang", 3);
	flt_vang = new Filter("gyro", 3);
	flt_vang_hpf = new Filter("gyro_hpf", 3);

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
	if (enabled->get()==0.0)
		return true;

	double ang = devs->getDeviceValue(Devices::angle);

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
	double tq;
	double pos;
	double posError;
	double posVError;
	double angError;
	double angVError;

	double ang;
	double angV;

	// Get position from device.
	pos = devs->getDeviceValue(Devices::pos);
	pos = filterDevice(flt_pos, pos);

	if (updateActualPosition) {
		// Get actual position from Devices
		sumError = 0;
		prevPosError = 0;
		prevAngError = 0;
		relPosOffset = pos;
		updateActualPosition = false;
	}

	// Determine position error
	posError = pos_sp->get() - (pos - relPosOffset);
	posVError = (posError - prevPosError);

	// Determine setpoint for angle.
	ang_sp->set(posError * ang_sp_kp->get());

	// Limit angle setpoint between -1 and 1 degree
	if (ang_sp->get() <-1) ang_sp->set(-1);
	if (ang_sp->get() > 1) ang_sp->set( 1);

	// Get angle from Device
	ang = devs->getDeviceValue(Devices::angle);
	ang = filterDevice(flt_ang, ang);
	ang_flt->set(ang);

	// Determine angle error
	angError = ang_sp->get() - ang;
	angVError = (angError - prevAngError);

	// Get angular velocity from gyro Device
	angV = devs->getDeviceValue(Devices::gyro);
	angV = filterDevice(flt_vang, angV);
	angV = filterDevice(flt_vang_hpf, angV);
	vang_flt->set(angV);

	// Determine integrated position error
	sumError += posError;
	if ((posError > 0 && prevPosError < 0)
			|| (posError < 0 && prevPosError > 0))
		sumError = 0;


	// Store individual controller contributions
	co_poskp->set(pos_kp->get() * posError);
	co_poskd->set(pos_kd->get() * posVError);
	co_poski->set(pos_ki->get() * sumError);

	co_angkp->set(ang_kp->get() * angError);
	co_angkd->set(ang_kd->get() * angV);
	co_angkd->set(angV); // TODO: remove this line

	// Calculate final torque
	tq = (pos_kp->get() * posError +
          pos_kd->get() * posVError +
		  pos_ki->get() * sumError +
		  ang_kp->get() * angError +
		  ang_kd->get() * angV);


	// Inject noise
	tq = doInject(tq);

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

void Controller::sample(void* /* context */) {
	Controller* me = Controller::getInstance(); // static_cast<Controller*>(context);
	me->sample();
}
