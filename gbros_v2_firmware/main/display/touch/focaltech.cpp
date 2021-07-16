/////////////////////////////////////////////////////////////////
/*
MIT License

Copyright (c) 2019 lewis he

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

focaltech.cpp - Arduino library for focaltech chip, support FT5206,FT6236,FT5336,FT6436L,FT6436
Created by Lewis on April 17, 2019.
github:https://github.com/lewisxhe/FocalTech_Library
*/
/////////////////////////////////////////////////////////////////
#include "focaltech.h"
extern "C" {
	#include "utils/software_i2c.h"
}

#ifdef CONFIG_SCREEN_ROTATION
	#define	CONFIG_ROTATION 			CONFIG_SCREEN_ROTATION
#else
	#define	CONFIG_ROTATION 			0
#endif

#define FT5206_VENDID                   (0x11)
#define FT6206_CHIPID                   (0x06)
#define FT6236_CHIPID                   (0x36)
#define FT6236U_CHIPID                  (0x64)
#define FT5206U_CHIPID                  (0x64)

#define FOCALTECH_REGISTER_MODE         (0x00)
#define FOCALTECH_REGISTER_GEST         (0x01)
#define FOCALTECH_REGISTER_STATUS       (0x02)
#define FOCALTECH_REGISTER_TOUCH1_XH    (0x03)
#define FOCALTECH_REGISTER_TOUCH1_XL    (0x04)
#define FOCALTECH_REGISTER_TOUCH1_YH    (0x05)
#define FOCALTECH_REGISTER_TOUCH1_YL    (0x06)
#define FOCALTECH_REGISTER_THRESHHOLD   (0x80)
#define FOCALTECH_REGISTER_MONITORTIME  (0x87)

#define FOCALTECH_REGISTER_LIB_VERSIONH (0xA1)
#define FOCALTECH_REGISTER_LIB_VERSIONL (0xA2)
#define FOCALTECH_REGISTER_INT_STATUS   (0xA4)
#define FOCALTECH_REGISTER_POWER_MODE   (0xA5)
#define FOCALTECH_REGISTER_VENDOR_ID    (0xA3)
#define FOCALTECH_REGISTER_VENDOR1_ID   (0xA8)
#define FOCALTECH_REGISTER_ERROR_STATUS (0xA9)

#define ACK_VAL                     false                     /*!< I2C ack value */
#define NACK_VAL                    true                     /*!< I2C nack value */

#define WRITE_BIT					0
#define READ_BIT					1

#define CONFIG_WIDTH  				CONFIG_SCREEN_WIDTH
#define CONFIG_HEIGHT 				CONFIG_SCREEN_HEIGHT

#define TOUCH_SOFTI2C_SDA				CONFIG_TOUCH_SOFTI2C_SDA
#define TOUCH_SOFTI2C_SCL				CONFIG_TOUCH_SOFTI2C_SCL

bool FocalTech_Class::probe(void)
{
#ifdef ARDUINO
    _i2cPort->beginTransmission(_address);
    if (_i2cPort->endTransmission() == 0) {
        initialization = true;
    }
#else
    initialization = true;
#endif
    return true;
}
/*#ifdef ARDUINO
bool FocalTech_Class::begin(TwoWire &port, uint8_t addr)
{
    _i2cPort = &port;
    _address = addr;
    _readCallbackFunc = nullptr;
    _writeCallbackFunc = nullptr;
    return probe();
}
#endif*/

bool FocalTech_Class::begin(iic_com_fptr_t read_cb, iic_com_fptr_t write_cb, uint8_t addr)
{
    if (read_cb == nullptr || write_cb == nullptr) {
        return false;
    }
    _readCallbackFunc = read_cb;
    _writeCallbackFunc = write_cb;
    _address = addr;
    return probe();
}

uint8_t FocalTech_Class::getDeviceMode(void)
{
    if (!initialization) {
        return 0;
    }
    return readRegister8(FOCALTECH_REGISTER_MODE) & 0x03;
}

GesTrue_t FocalTech_Class::getGesture(void)
{
    if (!initialization) {
        return FOCALTECH_NO_GESTRUE;
    }
    uint8_t val = readRegister8(FOCALTECH_REGISTER_GEST);
    switch (val) {
    case 0x10:
        return FOCALTECH_MOVE_UP;
    case 0x14:
        return FOCALTECH_MOVE_RIGHT;
    case 0x18:
        return FOCALTECH_MOVE_DOWN;
    case 0x1C:
        return FOCALTECH_MOVE_LEFT;
    case 0x48:
        return FOCALTECH_ZOOM_IN;
    case 0x49:
        return FOCALTECH_ZOOM_OUT;
    default:
        break;
    }
    return FOCALTECH_NO_GESTRUE;
}

void FocalTech_Class::setTheshold(uint8_t value)
{
    if (!initialization) {
        return;
    }
    writeRegister8(FOCALTECH_REGISTER_THRESHHOLD, value);
}

uint8_t FocalTech_Class::getThreshold(void)
{
    if (!initialization) {
        return 0;
    }
    return readRegister8(FOCALTECH_REGISTER_THRESHHOLD);
}

uint8_t FocalTech_Class::getMonitorTime(void)
{
    if (!initialization) {
        return 0;
    }
    return readRegister8(FOCALTECH_REGISTER_MONITORTIME);
}

void FocalTech_Class::setMonitorTime(uint8_t sec)
{
    if (!initialization) {
        return;
    }
    writeRegister8(FOCALTECH_REGISTER_MONITORTIME, sec);
}

void FocalTech_Class::enableAutoCalibration(void)
{
    if (!initialization) {
        return;
    }
    writeRegister8(FOCALTECH_REGISTER_MONITORTIME, 0x00);
}

void FocalTech_Class::disableAutoCalibration(void)
{
    if (!initialization) {
        return;
    }
    writeRegister8(FOCALTECH_REGISTER_MONITORTIME, 0xFF);
}

void FocalTech_Class::getLibraryVersion(uint16_t &version)
{
    if (!initialization) {
        return;
    }
    uint8_t buffer[2];
    readBytes(FOCALTECH_REGISTER_LIB_VERSIONH, buffer, 2);
    version = (buffer[0] << 8) | buffer[1];
}

void FocalTech_Class::enableINT(void)
{
    if (!initialization) {
        return;
    }
    writeRegister8(FOCALTECH_REGISTER_INT_STATUS, 0);
}

void FocalTech_Class::disableINT(void)
{
    if (!initialization) {
        return;
    }
    writeRegister8(FOCALTECH_REGISTER_INT_STATUS, 1);
}

bool FocalTech_Class::getPoint(uint16_t &x, uint16_t &y)
{
    if (!initialization) {
        return false;
    }
    uint8_t buffer[5];
    if (!readBytes(FOCALTECH_REGISTER_STATUS, buffer, 5)) {
        if (buffer[0] == 0 || buffer[0] > 2) {
            return false;
        }
        event = (EventFlag_t)(buffer[1] & 0xC0);
        x = ((buffer[1] & 0x0F) << 8 | buffer[2]); // @suppress("Symbol is not resolved")
        y = ((buffer[3] & 0x0F) << 8 | buffer[4]); // @suppress("Symbol is not resolved")
        if(!CONFIG_ROTATION){
        	x = CONFIG_HEIGHT - x; // @suppress("Symbol is not resolved")
        	y = CONFIG_WIDTH - y; // @suppress("Symbol is not resolved")
        }

#ifdef DBG_FOCALTECH
        printf("x=%03u y=%03u\n", x, y);
#endif
        return true;
    }
    return false;
}

uint8_t FocalTech_Class::getTouched()
{
    if (!initialization) {
        return 0;
    }
    return readRegister8(FOCALTECH_REGISTER_STATUS);
}

void FocalTech_Class::setPowerMode(PowerMode_t m)
{
    if (!initialization) {
        return;
    }
    writeRegister8(FOCALTECH_REGISTER_POWER_MODE, m);
}

uint8_t FocalTech_Class::getVendorID(void)
{
    if (!initialization) {
        return 0;
    }
    return readRegister8(FOCALTECH_REGISTER_VENDOR_ID);
}

uint8_t FocalTech_Class::getVendor1ID(void)
{
    if (!initialization) {
        return 0;
    }
    return readRegister8(FOCALTECH_REGISTER_VENDOR1_ID);
}

uint8_t FocalTech_Class::getErrorCode(void)
{
    if (!initialization) {
        return 0;
    }
    return readRegister8(FOCALTECH_REGISTER_ERROR_STATUS);
}

// bool FocalTech_Class::getPoint(uint8_t *x, uint8_t *y)
// {
//     if (!initialization || x == nullptr || y == nullptr) {
//         return false;
//     }
//     // uint16_t _id[2];
//     uint8_t buffer[16];
//     readBytes(FOCALTECH_REGISTER_MODE, buffer, 16);
//     if (buffer[FOCALTECH_REGISTER_STATUS] == 0) {
//         return false;
//     }
//     for (uint8_t i = 0; i < 2; i++) {
//         x[i] = buffer[FOCALTECH_REGISTER_TOUCH1_XH + i * 6] & 0x0F;
//         x[i] <<= 8;
//         x[i] |= buffer[FOCALTECH_REGISTER_TOUCH1_XL + i * 6];
//         y[i] = buffer[FOCALTECH_REGISTER_TOUCH1_YH + i * 6] & 0x0F;
//         y[i] <<= 8;
//         y[i] |= buffer[FOCALTECH_REGISTER_TOUCH1_YL + i * 6];
//         // _id[i] = buffer[FOCALTECH_REGISTER_TOUCH1_YH + i * 6] >> 4;
//     }
//     return true;
// }

uint8_t FocalTech_Class::readRegister8(uint8_t reg)
{
    uint8_t value;
    (void)readBytes(reg, &value, 1);
    return value;
}

void FocalTech_Class::writeRegister8(uint8_t reg, uint8_t value)
{
    (void)writeBytes(reg, &value, 1);
}

bool FocalTech_Class::readBytes( uint8_t reg, uint8_t *data, uint8_t nbytes)
{
    if (_readCallbackFunc != nullptr) {
        return _readCallbackFunc(_address, reg, data, nbytes);
    }
#ifdef ARDUINO
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();
    _i2cPort->requestFrom(_address, nbytes);
    uint8_t index = 0;
    while (_i2cPort->available())
        data[index++] = _i2cPort->read();
#endif
    return false;
}

bool FocalTech_Class::writeBytes( uint8_t reg, uint8_t *data, uint8_t nbytes)
{
    if (_writeCallbackFunc != nullptr) {
        return _writeCallbackFunc(_address, reg, data, nbytes);
    }
#ifdef ARDUINO
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    for (uint8_t i = 0; i < nbytes; i++) {
        _i2cPort->write(data[i]);
    }
    return _i2cPort->endTransmission() != 0;
#endif
    return false;
}

unsigned char touch_read(unsigned char dev_addr, short unsigned int reg_addr, unsigned char *data, unsigned char len)
{
    if (len == 0) {
        return ESP_OK;
    }
    if (data == NULL) {
        return ESP_FAIL;
    }
    sw_i2c_master_start();
    sw_i2c_master_write_byte( (dev_addr << 1) | WRITE_BIT);
    sw_i2c_master_write_byte( reg_addr);
    esp_err_t ret =  sw_i2c_master_stop();
    if (ret != ESP_OK) {
        return ESP_FAIL;
    }
    sw_i2c_master_start();
    sw_i2c_master_write_byte( (dev_addr << 1) | READ_BIT);
    if (len > 1) {
    	sw_i2c_master_read( data, len - 1, ACK_VAL);
    }
    sw_i2c_master_read_byte( &data[len - 1], NACK_VAL);
    sw_i2c_master_stop();
    ret = sw_i2c_master_stop();
    return ret;
}

/**
 * @brief apx library i2c write callblack
 */
unsigned char touch_write(unsigned char dev_addr, short unsigned int  reg_addr, unsigned char *data, unsigned char len)
{
    if (data == NULL) {
        return ESP_FAIL;
    }
    sw_i2c_master_start();
    sw_i2c_master_write_byte( (dev_addr << 1) | WRITE_BIT);
    sw_i2c_master_write_byte( reg_addr);
    sw_i2c_master_write( data, len);
    esp_err_t ret=sw_i2c_master_stop();
    return ret;
}

FocalTech_Class initTouch(){
	FocalTech_Class touch;
	sw_i2c_init(TOUCH_SOFTI2C_SDA, TOUCH_SOFTI2C_SCL); // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
	if (!touch.begin(touch_read, touch_write, 0x38)) {
		printf("Touch not initialized!\n");
	}
	return touch;
}
