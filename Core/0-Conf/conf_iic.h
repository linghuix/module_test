/*
 * conf_spi.h
 *
 *  Created on: May 19, 2020
 *      Author: test
 */

#ifndef _CONF_IIC_H_
#define _CONF_IIC_H_

#include "device_bsp.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;


void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

#endif /* BSP_CONF_SPI_H_ */

