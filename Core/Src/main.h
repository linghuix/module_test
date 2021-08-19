
#ifndef __MAIN_H
#define __MAIN_H

#include  "BSP.h"			// Board support package header


//#define ITM_dbg


#ifndef ITM_dbg
#include "debug.h"
#else
#include "prinf_debug_ITM.h"
#endif

#include "FSR.h"

//#include "func_1912.h"

#include "hard_test.h"

#include "func_RS485.h"
#include "ultrasonic_sensor.h"
#include "led.h"
#include "systemTimer.h"
#include "Servos.h"
#include "func_can.h"
#include "M8128ForceCollector.h"
#include "func_accelerate.h"
#include "func_dac.h"
#include "func_adc.h"
#include "ads1115.h"
//#include "Beijing_driver_control_function.h"
#include "Odrive.h"
#include "func_beep.h"

#include "DS_300c.h"

#include "func_w25qxx.h"
#include "func_spi.h"



#include "win.h"


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
