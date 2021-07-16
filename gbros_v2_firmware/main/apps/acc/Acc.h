/*
 * Clock.h
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#ifndef MAIN_APPS_ACC_H_
#define MAIN_APPS_ACC_H_

#include "../../display/TFT/SPIDisplay.h"
#include "../../sensors/PMU/axp20x.h"
#include "../../sensors/accel/bma.h"

#include "../App.h"

class Acc{
public:
	Acc(App * appl, SPIDisplay disp, AXP20X_Class pmu, BMA423 accel);
	void run(void);
private:
	SPIDisplay tft;
	AXP20X_Class axp;
	BMA423 acc;
	App * app;
	int getElapsedSeconds(int t1, int t2);
	bool drawAcc(Accel data, Accel dataOld);
};

#endif /* MAIN_APPS_CLOCK_H_ */
