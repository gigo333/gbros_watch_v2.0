/*
 * Timer.cpp
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#include "../timer/Timer.h"

/*static void taskWrapper(void* Param) {
    static_cast<Timer *>(Param)->clockTask();
    vTaskDelete(NULL);
}*/

Timer::Timer(App * appl, SPIDisplay disp, AXP20X_Class pmu, FocalTech_Class focalTech, PCF8563_Class rtc) {
	app=appl;
	tft=disp;
	axp=pmu;
	touch=focalTech;
	clock=rtc;
	elapsedTime.second=15;
}

void Timer::run() {
	uint16_t x=0, y=0;
	bool touched;
	char * format;
	RTC_Time timeOld = elapsedTime;
	app->start();
	tft.enableFontFill(BLACK);
	tft.FillScreen();
	tft.DrawFillRect(90, 10, 150, 70, RED);
	format = new char[16];
	snprintf(format, 16, "%02d:%02d:%02d", elapsedTime.hour, elapsedTime.minute, elapsedTime.second);
	tft.DrawString(56, 130, format, WHITE);
	delete format;
	drawTime(elapsedTime, timeOld);
	while(app->isRunning()){
		if(app->isNotifying()){
			while(app->isNotifying()){
				tft.DelayMS(20);
			}
			snprintf(format, 16, "%02d:%02d:%02d", elapsedTime.hour, elapsedTime.minute, elapsedTime.second);
			tft.DrawString(56, 130, format, WHITE);
			tft.DrawFillRect(90, 10, 150, 70, RED);
		}
		if(taskRunning && drawTime(elapsedTime, timeOld)){
			timeOld=elapsedTime;
		}
		touched = touch.getPoint(x,y);
		if(touched){
			if((x>90 && x<150) && (y>10 && y<70)){
				onBtn0Clicked();
				vTaskDelay(500 / portTICK_PERIOD_MS);
			}
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
	tft.disableFontFill();
}

void Timer::onBtn0Clicked(){
	if(!taskRunning){
		if (elapsedTime.second<10 && elapsedTime.minute==0 && elapsedTime.hour==0){
			char format[16];
			elapsedTime.second=0;
			elapsedTime.minute=1;
			snprintf(format, 16, "%02d:%02d:%02d", elapsedTime.hour, elapsedTime.minute, elapsedTime.second);
			tft.DrawString(56, 130, format, WHITE);
		} else{
			start();
			tft.DrawFillRect(90, 10, 150, 70, GREEN);
		}
	} else {
		stop();
		tft.DrawFillRect(90, 10, 150, 70, RED);
	}
}

void Timer::start(){
	if(!taskRunning && !elapsedTime.expired()){
		taskRunning=true;
		xTaskCreate(this->startTaskImpl, "timer", 4096, this, 5, &task);
	}
}

void Timer::stop(){
	if(taskRunning){
		vTaskDelete(task);
		taskRunning=false;
	}
}

/*int Timer::getElapsedSeconds(int t1, int t2){
	if (t2>t1)
		t2=t2-60;
	return t1-t2;
}*/

void Timer::timerTask(void){
	uint8_t seconds= clock.getSeconds();
	uint8_t secondsOld= clock.getSeconds();
	while(!elapsedTime.expired()){
		if(seconds!=secondsOld){
			secondsOld=seconds;
			elapsedTime.decrement();
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
		seconds=clock.getSeconds();
	}
	char s[] = "TIMER";
	app->startNotify(axp, tft, s, WHITE, BLACK, 500, 500);
	taskRunning=false;
}

void Timer::startTaskImpl(void* Param){
	static_cast<Timer *>(Param)->timerTask();
	vTaskDelete(NULL);
}

bool Timer::drawTime(RTC_Time time, RTC_Time timeOld){
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

