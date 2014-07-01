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
, devs(0)
, motor(0)

, par_alfa(createParameter("alfa"))
, par_alfa_dot(createParameter("alfa_dot"))
, par_beta(createParameter("beta"))
, par_x(createParameter("x"))
, par_x_dot(createParameter("x_dot"))

, par_x_kp(createParameter("x_kp"))
, par_x_kd(createParameter("x_kd"))
, par_alfa_kp(createParameter("alfa_kp"))
, par_alfa_kd(createParameter("alfa_kd"))

, par_out(createParameter("out"))
, par_out_x_kp(createParameter("out_x_kp"))
, par_out_x_kd(createParameter("out_x_kd"))
, par_out_alfa_kp(createParameter("out_alfa_kp"))
, par_out_alfa_kd(createParameter("out_alfa_kd"))

, par_enabled(createParameter("enabled"))

, dev_enc(0)
, dev_enc_d(0)
, dev_acc(0)
, dev_gyro(0)
{
	updateActualPosition = true;
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::setDevices(Devices* devs) {
	this->devs = devs;
	dev_gyro = devs->getDevice(Devices::GYRO);
	dev_acc = devs->getDevice(Devices::ACC);
	dev_enc = devs->getDevice(Devices::ENCPOS);
	dev_enc_d = devs->getDevice(Devices::ENCPOS_D);

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
	if (*par_enabled==0.0)
		return true;

#warning Implement MMDC safe

	return true;
}

void Controller::disableController()
{
	motor->setTorque(0.0);
	*par_enabled = 0.0;
	updateActualPosition = true;
}

double Controller::getFrequency()
{
	int par_idx = Parameter::findParameter("CmdUpdateFrequency.frequency");

	return Parameter::getByIdx(par_idx);
}

void Controller::calculateModel()
{
	static double alfa(0.0);
	static double t1(0.0);
	static double t2(0.0);
	static double freq(1.0);
	static double c1(0.0);
	static double c2(0.0);
	static double x_offset;
	static double x;
	static double x_dot;

	double kpx(0.0);
	double kdx(0.0);
	double kpa(0.0);
	double kda(0.0);
	double out(0.0);

	if (updateActualPosition) {
		// Get actual position from Devices
		updateActualPosition = false;
		freq = getFrequency();
		alfa = 0.0;
		x_offset = *dev_enc;
	}

	// Todo: move this inside updateActualPosition
	freq = getFrequency();

	t1 = *dev_acc * (1.0 - *par_beta);

	c1 = *dev_gyro * 1.0/freq;
	c2 = alfa + c1;
	t2 = c2 * (*par_beta);

	alfa = t1 + t2;

	*par_alfa = alfa;
	*par_alfa_dot = *dev_gyro;

	x = *dev_enc - x_offset;
	x_dot = *dev_enc_d;

	*par_x = x;
	*par_x_dot = x_dot;

	// Calculate PID for alfa and x

	kpx = *par_x_kp * x;
	kdx = *par_x_kd * x_dot;
	kpa = *par_alfa_kp * alfa;
	kda = *par_alfa_kd * (*dev_gyro);

	out = kpx + kdx + kpa + kda;

	*par_out = out;
	*par_out_x_kp = kpx;
	*par_out_x_kd = kdx;
	*par_out_alfa_kp = kpa;
	*par_out_alfa_kd = kda;

	if (*par_enabled!=0.0) {
		motor->setDC(out);
	}
	else {
		updateActualPosition = true;
	}
}

