/*
 * SPIDisplay.cpp
 *
 *  Created on: 6 lug 2021
 *      Author: user
 */

#include "SPIDisplay.h"

#define CONFIG_WIDTH  				CONFIG_SCREEN_WIDTH
#define CONFIG_HEIGHT 				CONFIG_SCREEN_HEIGHT
#define CONFIG_MOSI_GPIO 			CONFIG_SCREEN_MOSI_GPIO
#define CONFIG_SCLK_GPIO 			CONFIG_SCREEN_SCLK_GPIO
#define CONFIG_CS_GPIO 				CONFIG_SCREEN_CS_GPIO
#define CONFIG_DC_GPIO				CONFIG_SCREEN_DC_GPIO
#define CONFIG_RESET_GPIO			CONFIG_SCREEN_RESET_GPIO
#define CONFIG_BL_GPIO				CONFIG_SCREEN_BL_GPIO
#define CONFIG_OFFSETX 				CONFIG_SCREEN_OFFSETX
#define	CONFIG_OFFSETY 				CONFIG_SCREEN_OFFSETX
#ifdef CONFIG_SCREEN_ROTATION
	#define	CONFIG_ROTATION 			CONFIG_SCREEN_ROTATION
#else
	#define	CONFIG_ROTATION 			0
#endif

void SPIDisplay::begin(){
	notify=false;
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
    lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY, CONFIG_ROTATION);
	InitFontx(fx16G,"/spiffs/ILGH16XB.FNT",""); // 8x16Dot Gothic
	InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic
	InitFontx(fx32L,"/spiffs/LATIN32B.FNT",""); // 16x32Dot Latin
	InitFontx(fx16M,"/spiffs/ILMH16XB.FNT",""); // 8x16Dot Mincyo
	InitFontx(fx24M,"/spiffs/ILMH24XB.FNT",""); // 12x24Dot Mincyo
	InitFontx(fx32M,"/spiffs/ILMH32XB.FNT",""); // 16x32Dot Mincyo
	for(int i=0; i<2; i++)
		font[i]=fx32G[i];
}

void SPIDisplay::selectFont(uint8_t fontCode){
	if (!notify){
		int i;
		if(fontCode==FONT_GOTHIC_8x16)
			for(i=0; i<2; i++)
				font[i]=fx16G[i];
		else if(fontCode==FONT_GOTHIC_12x24)
			for(i=0; i<2; i++)
				font[i]=fx24G[i];
		else if(fontCode==FONT_GOTHIC_16x32)
			for(i=0; i<2; i++)
				font[i]=fx32G[i];
		else if(fontCode==FONT_LATIN_16x32)
			for(i=0; i<2; i++)
				font[i]=fx32L[i];
		else if(fontCode==FONT_MINCYO_8x16)
			for(i=0; i<2; i++)
				font[i]=fx16M[i];
		else if(fontCode==FONT_MINCYO_12x24)
			for(i=0; i<2; i++)
				font[i]=fx24M[i];
		else if(fontCode==FONT_MINCYO_16x32)
			for(i=0; i<2; i++)
				font[i]=fx32M[i];
	}
}

void SPIDisplay::enableFontFill(uint16_t color){
	if (!notify){
		dev._font_fill=1;
		dev._font_fill_color=color;
	}
}

void SPIDisplay::disableFontFill(){
	if (!notify)
		dev._font_fill=0;
}

void SPIDisplay::startNotify(){
	notify=true;
	delayMS(1000); //delay to let the display finish drawing before notifying (otherways the firmware crashes)
}

void SPIDisplay::stopNotify(){
	notify=false;
}

void SPIDisplay::DelayMS(int ms) {
	delayMS(ms);
}

/*void SPIDisplay::Init(int width, int height, int offsetx, int offsety){
	lcdInit(&dev, width, height, offsetx, offsety);
}*/

void SPIDisplay::DrawPixel(uint16_t x, uint16_t y, uint16_t color){
	if(!notify)
		lcdDrawPixel(&dev, x, y, color);
}

void SPIDisplay::DrawMultiPixels(uint16_t x, uint16_t y, uint16_t size, uint16_t * colors){
	if(!notify)
		lcdDrawMultiPixels(&dev, x, y, size, colors);
}

void SPIDisplay::DrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	if(!notify)
		lcdDrawFillRect(&dev, x1, y1, x2, y2, color);
}

void SPIDisplay::DisplayOff(void){
	if(!notify)
		lcdDisplayOff(&dev);
}

void SPIDisplay::DisplayOn(void){
	if(!notify)
		lcdDisplayOn(&dev);
}

void SPIDisplay::FillScreen(uint16_t color){
	if(!notify)
		lcdFillScreen(&dev, color);
}

void SPIDisplay::FillScreen(){
	if(!notify && dev._font_fill)
		lcdFillScreen(&dev, dev._font_fill_color);
}

void SPIDisplay::__FillScreen(uint16_t color){
	lcdFillScreen(&dev, color);
}

void SPIDisplay::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	if(!notify)
		lcdDrawLine(&dev, x1, y1, x2, y2, color);
}

void SPIDisplay::DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	if(!notify)
		lcdDrawRect(&dev, x1, y1, x2, y2, color);
}

void SPIDisplay::DrawRectAngle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color){
	if(!notify)
		lcdDrawRectAngle(&dev, xc, yc, w, h, angle, color);
}

void SPIDisplay::DrawTriangle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color){
	if(!notify)
		lcdDrawTriangle(&dev, xc, yc, w, h, angle, color);
}

void SPIDisplay::DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color){
	if(!notify)
		lcdDrawCircle(&dev, x0, y0, r, color);
}

void SPIDisplay::DrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color){
	if(!notify)
		lcdDrawFillCircle(&dev, x0, y0, r, color);
}

void SPIDisplay::DrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color){
	if(!notify)
		lcdDrawRoundRect(&dev, x1, y1, x2, y2, r, color);
}

void SPIDisplay::DrawArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color){
	if(!notify)
		lcdDrawArrow(&dev, x0, y0, x1, y1, w, color);
}

void SPIDisplay::DrawFillArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color){
	if(!notify)
		lcdDrawFillArrow(&dev, x0, y0, x1, y1, w, color);
}

uint16_t SPIDisplay::RGB565_conv(uint16_t r, uint16_t g, uint16_t b){
	return rgb565_conv(r, g, b);
}

int SPIDisplay::DrawChar(uint16_t x, uint16_t y, char ascii, uint16_t color){
	if(!notify)
		return lcdDrawChar(&dev, font, x, y, (uint8_t) ascii, color);
	return 1;
}

int SPIDisplay::DrawString(uint16_t x, uint16_t y, char * ascii, uint16_t color){
	if(!notify)
		return lcdDrawString(&dev, font, x, y, (uint8_t *)ascii, color);
	return 1;
}

int SPIDisplay::__DrawString(uint16_t x, uint16_t y, char * ascii, uint16_t color, uint16_t fillColor){
	uint16_t fillOld = dev._font_fill;
	uint16_t colorOld = dev._font_fill_color;
	int res;
	dev._font_fill = 1;
	dev._font_fill_color = fillColor;
	res=lcdDrawString(&dev, fx32G, x, y, (uint8_t *)ascii, color);
	dev._font_fill = fillOld;
	dev._font_fill_color = colorOld;
	return res;
}

int SPIDisplay::DrawCode(uint16_t x,uint16_t y,uint8_t code,uint16_t color){
	if(!notify)
		return lcdDrawCode(&dev, font, x, y, code, color);
	return 1;
}

void SPIDisplay::SetFontDirection(uint16_t dir){
	if(!notify)
		lcdSetFontDirection(&dev, dir);
}

void SPIDisplay::SetFontFill(uint16_t color){
	if(!notify)
		lcdSetFontFill(&dev, color);
}

void SPIDisplay::UnsetFontFill(void){
	if(!notify)
		lcdUnsetFontFill(&dev);
}

void SPIDisplay::SetFontUnderLine(uint16_t color){
	if(!notify)
		lcdSetFontUnderLine(&dev ,color);
}

void SPIDisplay::UnsetFontUnderLine(void){
	if(!notify)
		lcdUnsetFontUnderLine(&dev);
}

void SPIDisplay::BacklightOff(void){
	if(!notify)
		lcdBacklightOff(&dev);
}

void SPIDisplay::BacklightOn(void){
	if(!notify)
		lcdBacklightOn(&dev);
}

void SPIDisplay::InversionOff(void){
	if(!notify)
		lcdInversionOff(&dev);
}

void SPIDisplay::InversionOn(void){
	if(!notify)
		lcdInversionOn(&dev);
}
