/*
 * conf_spi.h
 *
 *  Created on: May 19, 2020
 *      Author: test
 */

#ifndef BSP_CONF_SPI_H_
#define BSP_CONF_SPI_H_

#include "device_bsp.h"

void MX_SPIx_Init(SPI_TypeDef * SPI, int masterOrSlave);
	
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

#endif /* BSP_CONF_SPI_H_ */
