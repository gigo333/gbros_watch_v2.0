/*
 * master_i2c.c
 *
 *  Created on: 6 lug 2021
 *      Author: user
 */

#include "driver/i2c.h"

esp_err_t i2c_master_init(void);

int twi_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);

int twi_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);
