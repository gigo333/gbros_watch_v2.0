/*
 * Acc.cpp
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#include "Acc.h"


Acc::Acc(App * appl, SPIDisplay disp, AXP20X_Class pmu, BMA423 accel) {
	app=appl;
	tft=disp;
	axp=pmu;
	acc=accel;
}

void Acc::run() {
	Accel data, dataOld;
	char s1[]="X: 0", s2[]="Y: 0", s3[]="Z: 0";
	app->start();
	acc.enableAccel(true);
	tft.enableFontFill(BLACK);
	tft.FillScreen();
	tft.DrawString(60, 80, s1, RED);
	tft.DrawString(60, 130, s2, GREEN);
	tft.DrawString(60, 180, s3, BLUE);
	acc.getAccel(data);
	dataOld=data;
	while(app->isRunning()){
		if(app->isNotifying()){
			while(app->isNotifying()){
				tft.DelayMS(20);
			}
			tft.DrawString(60, 80, s1, RED);
			tft.DrawString(60, 130, s2, GREEN);
			tft.DrawString(60, 180, s3, BLUE);
			acc.getAccel(data);
			dataOld=data;
		}
		acc.getAccel(data);
		if(drawAcc(data, dataOld)){
			dataOld=data;
		}
		tft.DelayMS(20);
	}
	tft.disableFontFill();
	acc.disableAccel();
}

int Acc::getElapsedSeconds(int t1, int t2){
	if (t2>t1)
		t2=t2-60;
	return t1-t2;
}

bool Acc::drawAcc(Accel data, Accel dataOld){
	bool changed =false;
	char format[16];
	if(data.x!=dataOld.x){
		changed=true;
		snprintf(format, sizeof(format), "%d    ", data.x);
		tft.DrawString(108, 80, format , RED);
	}
	if(data.y!=dataOld.y){
		changed=true;
		snprintf(format, sizeof(format), "%d    ", data.y);
		tft.DrawString(108, 130, format , GREEN);
	}
	if(data.z!=dataOld.z){
		changed=true;
		snprintf(format, sizeof(format), "%d    ", data.z);
		tft.DrawString(108, 180, format , BLUE);
	}
	return changed;

}
