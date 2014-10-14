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

#include <ReflexxesAPI.h>
#include <RMLPositionFlags.h>
#include <RMLPositionInputParameters.h>
#include <RMLPositionOutputParameters.h>

Controller::Controller(ServoModule* pre)
: ServoModule("Controller", pre)
, RML(new ReflexxesAPI(1, 1.0/100))
, IP(new  RMLPositionInputParameters(1))
, OP(new RMLPositionOutputParameters(1))

, par_enabled(createParameter("enabled"))

, devs(0)
, motor(0)

, par_alfa(createParameter("alfa"))
, par_alfa_dot(createParameter("alfa_dot"))
, par_alfa_int(createParameter("alfa_int"))
, par_beta(createParameter("beta"))
, par_x(createParameter("x"))
, par_x_dot(createParameter("x_dot"))
, par_x_int(createParameter("x_int"))

, par_x_kp(createParameter("x_kp"))
, par_x_kd(createParameter("x_kd"))
, par_x_ki(createParameter("x_ki"))
, par_alfa_kp(createParameter("alfa_kp"))
, par_alfa_kd(createParameter("alfa_kd"))
, par_alfa_ki(createParameter("alfa_ki"))

, par_out(createParameter("out"))
, par_out_x_kp(createParameter("out_x_kp"))
, par_out_x_kd(createParameter("out_x_kd"))
, par_out_x_ki(createParameter("out_x_ki"))
, par_out_alfa_kp(createParameter("out_alfa_kp"))
, par_out_alfa_kd(createParameter("out_alfa_kd"))
, par_out_alfa_ki(createParameter("out_alfa_ki"))

, dev_enc(0)
, dev_enc_d(0)
, dev_acc(0)
, dev_gyro(0)

, par_reference(createParameter("reference"))
, par_target(createParameter("target"))

, par_maxA(createParameter("maxA"))
, par_maxV(createParameter("maxV"))
{
	updateActualPosition = true;
	std::cerr << "Construction Controller" << std::endl;
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
		calculateSetpoint();
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
	static double x(0.0);
	static double x_dot(0.0);
	static double x_int(0.0);
	static double alfa_int(0.0);

	static double xMin1(0.0);

	double kpx(0.0);
	double kdx(0.0);
	double kix(0.0);
	double kpa(0.0);
	double kda(0.0);
	double kia(0.0);
	double out(0.0);

	if (updateActualPosition) {
		// Get actual position from Devices
		updateActualPosition = false;
		freq = getFrequency();
		alfa = 0.0;
		x_offset = *dev_enc;
		alfa_int = 0.0;
		x_int=0.0;
		xMin1 = 0.0;
		resetSPG();
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

	x = (*dev_enc - x_offset)-(*par_reference);
	x_dot = (x - xMin1)*freq; //x_dot = *dev_enc_d;
	xMin1 = x;

	*par_x = x;
	*par_x_dot = x_dot;

	alfa_int = 0.99*alfa_int + alfa;
	*par_alfa_int = alfa_int;
	x_int = 0.99*x_int + x;
	*par_x_int = x_int;

	// Calculate PID for alfa and x
	kpx = *par_x_kp * x;
	kdx = *par_x_kd * x_dot;
	kix = *par_x_ki * x_int;
	kpa = *par_alfa_kp * alfa;
	kda = *par_alfa_kd * (*par_alfa_dot);
	kia = *par_alfa_ki * (*par_alfa_int);
	out = -(kpx + kdx + kpa + kda + kia + kix);

	*par_out = out;
	*par_out_x_kp = kpx;
	*par_out_x_kd = kdx;
	*par_out_x_ki = kix;
	*par_out_alfa_kp = kpa;
	*par_out_alfa_kd = kda;
	*par_out_alfa_ki = kia;

	if (*par_enabled!=0.0) {
		motor->setDC(out);
	}
	else {
		motor->setDC(0.0);
		updateActualPosition = true;
	}
}

void Controller::calculateSetpoint()
{
	static double prevTarget(0.0);
	static RMLPositionFlags flags;

	if (prevTarget != *par_target) {
		// Recalculate profile
		IP->MaxVelocityVector->VecData[0] = *par_maxV;
		IP->MaxAccelerationVector->VecData[0] = *par_maxA;
		IP->TargetPositionVector->VecData[0] = *par_target;
		prevTarget = *par_target;
	}

	RML->RMLPosition(*IP, OP, flags);
    *IP->CurrentPositionVector      =   *OP->NewPositionVector      ;
    *IP->CurrentVelocityVector      =   *OP->NewVelocityVector      ;
    *IP->CurrentAccelerationVector  =   *OP->NewAccelerationVector  ;

	*par_reference = OP->NewPositionVector->VecData[0];
}

void Controller::resetSPG()
{
	IP->CurrentPositionVector->VecData[0] = 0.0;
	IP->CurrentVelocityVector->VecData[0] = 0.0;
	IP->CurrentAccelerationVector->VecData[0] = 0.0;
	IP->MaxVelocityVector->VecData[0] = *par_maxV;
	IP->MaxAccelerationVector->VecData[0] = *par_maxA;
	IP->MaxJerkVector->VecData[0] = 1;
	IP->TargetPositionVector->VecData[0] = 0.0;
	IP->TargetVelocityVector->VecData[0] = 0.0;
	IP->SelectionVector->VecData[0] = true;

}
