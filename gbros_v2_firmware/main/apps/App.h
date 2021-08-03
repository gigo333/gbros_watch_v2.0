/*
 * App.h
 *
 *  Created on: 7 lug 2021
 *      Author: user
 */

#ifndef MAIN_APPS_APP_H_
#define MAIN_APPS_APP_H_

#include "../display/TFT/SPIDisplay.h"
#include "../sensors/PMU/axp20x.h"

class App {
public:
	bool isRunning(void);
	void start(void);
	void stop(void);
	void displayOff(void);
	void displayOn(void);
	bool isDisplayOff(void);
	bool isNotifying(void);
	void startNotify(AXP20X_Class axp, SPIDisplay tft, char * ascii, uint16_t color, uint16_t fillColor);
	void startNotify(AXP20X_Class axp, SPIDisplay tft, char * ascii, uint16_t color, uint16_t fillColor, int vibrationOnLen, int vibrationOffLen);
	void stopNotify(void);
private:
	bool running = false;
	bool dispOff = false;
	bool notify = false;
};

#endif /* MAIN_APPS_APP_H_ */
