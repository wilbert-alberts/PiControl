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
	static Controller* instance;
	Parameter* enabled;
	Motor* motor;
	bool updateActualPosition;
	Devices* devs;
};

#endif /* CONTROLLER_H_ */
