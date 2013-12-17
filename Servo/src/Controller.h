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

class Controller {
public:
	static Controller* getInstance();
	virtual ~Controller();

	static void sample(void* context);
	void sample();

private:
	Controller();
	void calculateModel();
	bool mmdcSafe();
	void disableController();

	static Controller* instance;
	Parameter* enabled;
	Parameter* pos_sp;
	Parameter* pos_kp;
	Parameter* pos_kd;
	Parameter* pos_ki;
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


	Motor* motor;
	bool updateActualPosition;
	Devices* devs;

	double prevPosError;
	double prevAngError;
	double relPosOffset;
	double sumError;

};

#endif /* CONTROLLER_H_ */
