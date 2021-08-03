/*
 * Timer.h
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#ifndef MAIN_APPS_TIMER_H_
#define MAIN_APPS_TIMER_H_

#include "../../display/TFT/SPIDisplay.h"
#include "../../sensors/PMU/axp20x.h"
#include "../../sensors/RTC/pcf8563.h"
#include "../../display/touch/focaltech.h"

#include "../App.h"

#include "freertos/Task.h"

class Timer{
public:
	Timer(App * appl, SPIDisplay disp, AXP20X_Class pmu, FocalTech_Class focalTech, PCF8563_Class rtc);
	void run(void);

private:
	SPIDisplay tft;
	AXP20X_Class axp;
	PCF8563_Class clock;
	App * app;
	RTC_Time elapsedTime;
	FocalTech_Class touch;
	TaskHandle_t task = NULL;
	bool taskRunning = false;
	//int getElapsedSeconds(int t1, int t2);
	bool drawTime(RTC_Time time, RTC_Time timeOld);
	static void startTaskImpl(void * Param);
	void timerTask(void);
	void start(void);
	void stop(void);
	void onBtn0Clicked(void);
};

#endif /* MAIN_APPS_CLOCK_H_ */
