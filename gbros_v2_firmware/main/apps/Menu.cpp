/*
 * Menu.cpp
 *
 *  Created on: 6 lug 2021
 *      Author: user
 */

#include "Menu.h"
#include "appIcons.h"
#include "freertos/Task.h"

Menu::Menu(SPIDisplay disp, FocalTech_Class tc) {
	// TODO Auto-generated constructor stub
	tft=disp;
	touch=tc;
}
uint8_t Menu::run() {
	tft.FillScreen(WHITE);
	int i, j;
	uint16_t x=0, y=0;
	int selectedApp=0;
	for(i=0; i<80; i++)
		tft.DrawMultiPixels(0 ,i , 80, watchIcon[i]);
	for(i=0; i<80; i++)
		tft.DrawMultiPixels(80,i , 80, accIcon[i]);
	for(i=0; i<80; i++)
		tft.DrawMultiPixels(160,i , 80, cameraIcon[i]);
	while(selectedApp==0){
		bool ret = touch.getPoint(x,y);
		if(ret){
			for (i=0; i<3; i++)
				for(j=0; j<3; j++)
					if (((x>i*80) && x<(i+1)*80) && ((y>j*80) && y<(j+1)*80))
						selectedApp=i+3*j+1;

		}
		if (selectedApp>2)
				selectedApp=0;
		vTaskDelay(20 / portTICK_PERIOD_MS);
	}
	return selectedApp;
}

