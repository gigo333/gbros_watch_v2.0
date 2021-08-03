/*
 * App.cpp
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#include "App.h"

#include "driver/gpio.h"

bool App::isRunning() {
	return running;
}

void App::start() {
	running=true;
}

void App::stop(){
	running=false;
}

void App::displayOff(){
	dispOff=true;
}
void App::displayOn(){
	dispOff=false;
}

bool App::isDisplayOff(){
	return dispOff;
}

bool App::isNotifying(){
	return notify;
}

void App::startNotify(AXP20X_Class axp ,SPIDisplay tft, char * ascii, uint16_t color, uint16_t fillColor){
	notify=true;
	tft.startNotify();
	tft.__FillScreen(fillColor);
	tft.__DrawString(80, 130, ascii, color, fillColor);
	if (dispOff){
		axp.setPowerOutPut(AXP202_LDO2, AXP202_ON);
		dispOff=false;
	}
	while(notify){
		tft.DelayMS(20);
	}
	tft.stopNotify();
	tft.FillScreen();
}

void App::startNotify(AXP20X_Class axp, SPIDisplay tft, char * ascii, uint16_t color, uint16_t fillColor, int vibrationOnLen, int vibrationOffLen){
	notify=true;
	tft.startNotify();
	tft.__FillScreen(fillColor);
	tft.__DrawString(80, 130, ascii, color, fillColor);
	if (dispOff){
		axp.setPowerOutPut(AXP202_LDO2, AXP202_ON);
		dispOff=false;
	}
	while(notify){
		gpio_set_level(GPIO_NUM_4, 1); // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
		tft.DelayMS(vibrationOnLen);
		gpio_set_level(GPIO_NUM_4, 0); // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
		tft.DelayMS(vibrationOffLen);

	}
	gpio_set_level(GPIO_NUM_4, 0); // @suppress("Invalid arguments") // @suppress("Symbol is not resolved")
	tft.stopNotify();
	tft.FillScreen();
}

void App::stopNotify(){
	notify=false;
}
