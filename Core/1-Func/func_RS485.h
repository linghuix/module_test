/*
 * accelerate.h
 *
 *  Created on: May 25, 2020
 *      Author: test
 */

#ifndef _FUNC_RS485_H_
#define _FUNC_RS485_H_

#include "conf_usart.h"


TEST test_RS485(void);

void IDLE_UART_IRQHandler(UART_HandleTypeDef *huart);

#endif /* 1_FUNC_ACCELERATE_H_ */
