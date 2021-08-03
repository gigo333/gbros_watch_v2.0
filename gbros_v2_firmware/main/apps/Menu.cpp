/*
 * Menu.cpp
 *
 *  Created on: 6 lug 2021
 *      Author: user
 */

#include "Menu.h"
#include "freertos/Task.h"

Menu::Menu(App * appl, SPIDisplay disp, FocalTech_Class tc) {
	tft=disp;
	touch=tc;
	app=appl;
}
uint8_t Menu::run() {
	uint16_t x=0, y=0;
	int i, j;
	int selectedApp=0;
	bool touched;
	drawIcons();
	while(selectedApp==0){
		if(app->isNotifying()){
			while(app->isNotifying()){
				vTaskDelay(20 / portTICK_PERIOD_MS);
			}
			drawIcons();
		}
		touched = touch.getPoint(x,y);
		if(touched){
			for (i=0; i<3; i++)
				for(j=0; j<3; j++)
					if (((x>i*80) && x<(i+1)*80) && ((y>j*80) && y<(j+1)*80))
						selectedApp=i+3*j+1;

		}
		vTaskDelay(20 / portTICK_PERIOD_MS);
	}
	return selectedApp;
}

void Menu::drawIcons(){
	FILE *fp;
		uint16_t *buffer;
		fp = fopen("/spiffs/Icons.icon", "rb");
		buffer=(uint16_t *)malloc(240*sizeof(uint16_t));

		for(int i=0; i<240; i++){
			fread(buffer, 240*sizeof(uint16_t), 1, fp);
			tft.DrawMultiPixels(0 ,i , 240, buffer);
		}

		fclose(fp);
		free(buffer);
}
