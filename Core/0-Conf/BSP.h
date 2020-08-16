#ifndef __BSP_H
#define	__BSP_H

/**
  * @brief  provide interface of CONF device and call in func_x.h.
  */

#include "conf_usart.h"
#include "conf_global.h"
#include "conf_tim.h"
#include "conf_rtc.h"
#include "conf_Jlink.h"
#include "conf_can.h"
#include "conf_dac.h"
#include "conf_adc.h"


/**
  * @brief  Prompt the start of device function. 
  *         Since USART has configured completely, function declared in debug.h can use here. 
  */

#include "debug.h"



void SysTick_init(void);
void BSP_Init(void);

#endif // __BSP_H
