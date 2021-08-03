/*
 * Clock.cpp
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#include "../clock/Clock.h"


/*static void taskWrapper(void* Param) {
    static_cast<Clock *>(Param)->clockTask();
    vTaskDelete(NULL);
}*/

Clock::Clock(App * appl, SPIDisplay disp, AXP20X_Class pmu, PCF8563_Class rtc) {
	app=appl;
	tft=disp;
	axp=pmu;
	clock=rtc;
}

void Clock::run() {
	RTC_Date time, timeOld;
	int secondBreak, breakTime=10;
	app->start();
	tft.enableFontFill(BLACK);
	tft.FillScreen();
	tft.DrawString(56, 130, clock.formatDateTime(PCF_TIMEFORMAT_HMS), WHITE);
	timeOld=clock.getDateTime();
	secondBreak=timeOld.second;
	while(app->isRunning()){
		if(app->isNotifying()){
			while(app->isNotifying()){
				tft.DelayMS(20);
			}
			tft.DrawString(56, 130, clock.formatDateTime(PCF_TIMEFORMAT_HMS), WHITE);
			timeOld=clock.getDateTime();
			secondBreak=timeOld.second;
		}
		if(!app->isDisplayOff()){
			time=clock.getDateTime();
			if(getElapsedSeconds(time.second, secondBreak)<breakTime){
				if(drawTime(time, timeOld)){
					timeOld=time;
				}
			} else{
				app->displayOff();
				axp.setPowerOutPut(AXP202_LDO2, AXP202_OFF);
			}
		} else {
			secondBreak=clock.getSeconds();
		}
		tft.DelayMS(100);
	}
	tft.disableFontFill();
}



int Clock::getElapsedSeconds(int t1, int t2){
	if (t2>t1)
		t2=t2-60;
	return t1-t2;
}

bool Clock::drawTime(RTC_Date time, RTC_Date timeOld){
	//char format[16];
	if(time.hour/10!=timeOld.hour/10){
		//snprintf(format, sizeof(format), "%02d", time.hour);
		tft.DrawChar(56, 130, '0'+time.hour/10, WHITE);
	}
	if(time.hour%10!=timeOld.hour%10){
		//snprintf(format, sizeof(format), "%02d", time.hour);
		tft.DrawChar(72, 130, '0'+time.hour%10, WHITE);
	}
	if(time.minute/10!=timeOld.minute/10){
		//snprintf(format, sizeof(format), "%02d", time.minute);
		tft.DrawChar(104, 130, '0'+time.minute/10, WHITE);
	}
	if(time.minute%10!=timeOld.minute%10){
		//snprintf(format, sizeof(format), "%02d", time.minute);
		tft.DrawChar(120, 130, '0'+time.minute%10, WHITE);
	}
	if(time.second/10!=timeOld.second/10){
		//snprintf(format, sizeof(format), "%02d", time.second);
		tft.DrawChar(152, 130, '0'+time.second/10, WHITE);
	}
	if(time.second%10!=timeOld.second%10){
		//snprintf(format, sizeof(format), "%02d", time.second);
		tft.DrawChar(168, 130, '0'+time.second%10, WHITE);
		return true;
	}
	return false;

}

