/*
 * device_bsp.h
 *
 *  Created on: Mar 2, 2020
 *      Author: test
 */

#ifndef __DEVICE_BSP_H_
#define __DEVICE_BSP_H_


#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "math.h"

#include "debug.h"

#define TEST void
#define TEST_ON


#define Error_Handler() ERROR(3,"init error!"); //while(1);

#endif /* BSP_DEVICE_BSP_H_ */
