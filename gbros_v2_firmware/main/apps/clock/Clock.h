/*
 * Clock.h
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#ifndef MAIN_APPS_CLOCK_H_
#define MAIN_APPS_CLOCK_H_

#include "../../display/TFT/SPIDisplay.h"
#include "../../sensors/PMU/axp20x.h"
#include "../../sensors/RTC/pcf8563.h"

#include "../App.h"

class Clock{
public:
	Clock(App * appl, SPIDisplay disp, AXP20X_Class pmu, PCF8563_Class rtc);
	void run(void);
private:
	SPIDisplay tft;
	AXP20X_Class axp;
	PCF8563_Class clock;
	App * app;
	int getElapsedSeconds(int t1, int t2);
	bool drawTime(RTC_Date time, RTC_Date timeOld);
};

#endif /* MAIN_APPS_CLOCK_H_ */
