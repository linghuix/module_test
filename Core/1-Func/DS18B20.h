/*
 * DS18B20.h
 *
 *  Created on: May 29, 2020
 *      Author: test
 */

#ifndef _TASK_DS18B20_H_
#define _TASK_DS18B20_H_


#include "conf_gpio.h"
#include "conf_tim.h"
void GetTemperature(void);



extern char temper[5];
extern float Temperature;

#endif /* 2_TASK_DS18B20_H_ */
