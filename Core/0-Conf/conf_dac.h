/*
 * conf_dac.h
 *
 *  Created on: Aug 14, 2020
 *      Author: test
 */

#ifndef _CONF_DAC_H_
#define _CONF_DAC_H_

#include "device_bsp.h"


extern DAC_HandleTypeDef hdac;




#define DAC_CHANNEL_NOTUSE 0x00000011U






void MX_DAC_Init(uint32_t DAC_CHANNEL1, uint32_t DAC_CHANNEL2, uint32_t Trigger_Mode);



#endif /* 0_CONF_CONF_DAC_H_ */
