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

class Menu {
public:
	Menu(SPIDisplay disp, FocalTech_Class tc);
	uint8_t run(void);

private:
	SPIDisplay tft;
	FocalTech_Class touch;
};

#endif /* MAIN_APPS_MENU_H_ */
