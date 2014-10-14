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

#include "ReflexxesAPI.h"
#include <RMLPositionFlags.h>
#include <RMLPositionInputParameters.h>
#include <RMLPositionOutputParameters.h>

#include "ServoModule.h"

class Parameter;
class Device;

class Controller : public ServoModule{
public:
	Controller(ServoModule* pre);
	virtual ~Controller();

	void calculateAfter();

	void setDevices(Devices* devs);

	void setMotor(Motor* motor) {
		this->motor = motor;
	}

private:
	void   calculateModel();
	void   calculateSetpoint();
	void   resetSPG();
	bool   mmdcSafe();
	void   disableController();
	double getFrequency();

	static Controller* instance;

	ReflexxesAPI* RML;
    RMLPositionInputParameters  *IP;
	RMLPositionOutputParameters *OP;



	Parameter* par_enabled;
	Devices*   devs;
	Motor*     motor;
	bool       updateActualPosition;

	Parameter* par_alfa;
	Parameter* par_alfa_dot;
	Parameter* par_alfa_int;

	Parameter* par_beta;

	Parameter* par_x;
	Parameter* par_x_dot;
	Parameter* par_x_int;

	Device*    dev_enc;
	Device*    dev_enc_d;

	Device*    dev_acc;
	Device*    dev_gyro;

	Parameter* par_x_kp;
	Parameter* par_x_kd;
	Parameter* par_x_ki;
	Parameter* par_alfa_kp;
	Parameter* par_alfa_kd;
	Parameter* par_alfa_ki;

	Parameter* par_out;
	Parameter* par_out_x_kp;
	Parameter* par_out_x_kd;
	Parameter* par_out_x_ki;
	Parameter* par_out_alfa_kp;
	Parameter* par_out_alfa_kd;
	Parameter* par_out_alfa_ki;

	Parameter* par_reference;
	Parameter* par_target;

	Parameter* par_maxV;
	Parameter* par_maxA;

};

#endif /* CONTROLLER_H_ */
