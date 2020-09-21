/*
 * accelerate.h
 *
 *  Created on: May 25, 2020
 *      Author: test
 */

#ifndef _FUNC_ACCELERATE_H_
#define _FUNC_ACCELERATE_H_

#include "conf_usart.h"


#define Force_huart huart2

#define Force_uart USART2


TEST test1912_forceCollector_communication(void);
TEST test_CheckSum(void);
TEST test_Command(void);
TEST test_searchID(void);
TEST test_GetForce(void);
void IDLE_UART_IRQHandler(UART_HandleTypeDef *huart);
#endif /* 1_FUNC_ACCELERATE_H_ */
