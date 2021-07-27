/*
 * Menu.h
 *
 *  Created on: 6 lug 2021
 *      Author: user
 */

#ifndef MAIN_APPS_MENU_H_
#define MAIN_APPS_MENU_H_

#include "../display/TFT/SPIDisplay.h"
#include "../display/touch/focaltech.h"

#include "App.h"

class Menu {
public:
	Menu(App * appl ,SPIDisplay disp, FocalTech_Class tc);
	uint8_t run(void);

private:
	SPIDisplay tft;
	FocalTech_Class touch;
	App *app;
	void drawIcons(void);
};

#endif /* MAIN_APPS_MENU_H_ */
