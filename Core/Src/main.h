
#ifndef __MAIN_H
#define __MAIN_H

#include  "BSP.h"			// Board support package header


//#define ITM_dbg


#ifndef ITM_dbg
#include "debug.h"
#else
#include "prinf_debug_ITM.h"
#endif


//#include "ultrasonic_sensor.h"
//#include "led.h"
//#include "systemTimer.h"
//#include "Servos.h"
//#include "func_can.h"
//#include "M8128ForceCollector.h"

#include "func_dac.h"
#include "func_adc.h"
#include "ads1115.h"
#include "Beijing_driver_control_function.h"
#include "func_1912.h"
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
