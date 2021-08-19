/*
 * func_BLE_HC05.h
 *
 *  Created on: May 11, 2021
 *      Author: test
 */

#ifndef _FUNC_RCVCMD_H_
#define _FUNC_RCVCMD_H_

#include "conf_usart.h"

// #define RcvCMD_huart huart1

void RcvCmd_Init(void);

extern uint32_t inc;

TEST test_RcvCMD_communication(void);


#endif /* 1_FUNC_FUNC_BLE_HC05_H_ */
