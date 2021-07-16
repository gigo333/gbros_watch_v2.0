/*
 * SPIDisplay.h
 *
 *  Created on: 6 lug 2021
 *      Author: user
 */

#ifndef MAIN_SPIDISPLAY_H_
#define MAIN_SPIDISPLAY_H_

extern "C" {
	#include "st7789.h"
	#include "fontx.h"
}

#define RED			0xf800
#define GREEN			0x07e0
#define BLUE			0x001f
#define BLACK			0x0000
#define WHITE			0xffff
#define GRAY			0x8c51
#define YELLOW			0xFFE0
#define CYAN			0x07FF
#define PURPLE			0xF81F

#define FONT_GOTHIC_8x16 	0// 8x16Dot Gothic
#define FONT_GOTHIC_12x24 	1// 12x24Dot Gothic
#define FONT_GOTHIC_16x32 	2// 16x32Dot Gothic
#define FONT_LATIN_16x32  	3// 16x32Dot Latin
#define FONT_MINCYO_8x16 	4// 8x16Dot Mincyo
#define FONT_MINCYO_12x24 	5// 12x24Dot Mincyo
#define FONT_MINCYO_16x32 	6// 16x32Dot Mincyo


#define DIRECTION0		0
#define DIRECTION90		1
#define DIRECTION180		2
#define DIRECTION270		3


class SPIDisplay {
public:
	void begin(void);
	void DelayMS(int ms);
	//void Init(int width, int height, int offsetx, int offsety);
	void DrawPixel(uint16_t x, uint16_t y, uint16_t color);
	void DrawMultiPixels(uint16_t x, uint16_t y, uint16_t size, uint16_t * colors);
	void DrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
	void DisplayOff(void);
	void DisplayOn(void);
	void FillScreen(uint16_t color);
	void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
	void DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
	void DrawRectAngle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color);
	void DrawTriangle(uint16_t xc, uint16_t yc, uint16_t w, uint16_t h, uint16_t angle, uint16_t color);
	void DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
	void DrawFillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
	void DrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint16_t color);
	void DrawArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
	void DrawFillArrow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t w, uint16_t color);
	uint16_t RGB565_conv(uint16_t r, uint16_t g, uint16_t b);
	int DrawChar(uint16_t x, uint16_t y, char ascii, uint16_t color);
	int DrawString(uint16_t x, uint16_t y, char * ascii, uint16_t color);
	int DrawCode(uint16_t x,uint16_t y,uint8_t code,uint16_t color);
	//int DrawUTF8Char(FontxFile *fx, uint16_t x, uint16_t y, uint8_t *utf8, uint16_t color);
	//int DrawUTF8String(FontxFile *fx, uint16_t x, uint16_t y, unsigned char *utfs, uint16_t color);
	void SetFontDirection(uint16_t dir);
	void SetFontFill(uint16_t color);
	void UnsetFontFill(void);
	void SetFontUnderLine(uint16_t color);
	void UnsetFontUnderLine(void);
	void BacklightOff(void);
	void BacklightOn(void);
	void InversionOff(void);
	void InversionOn(void);
	void selectFont(uint8_t fontCode);
	void enableFontFill(uint16_t color);
	void disableFontFill(void);
private:
	TFT_t dev;
    FontxFile fx16G[2];
    FontxFile fx24G[2];
    FontxFile fx32G[2];
    FontxFile fx32L[2];
	FontxFile fx16M[2];
	FontxFile fx24M[2];
	FontxFile fx32M[2];
	FontxFile font[2];
};

#endif /* MAIN_SPIDISPLAY_H_ */
