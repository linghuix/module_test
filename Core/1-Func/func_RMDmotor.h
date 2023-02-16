/**
 * func_motech.h
 *
 *  Created on: May 25, 2020
 *      Author: test
 */

#ifndef _FUNC_RMDMOTOR_H_
#define _FUNC_RMDMOTOR_H_

#include "conf_usart.h"
#include "math.h"

#define RMDmotor_huart huart1

void RMDmotor_Init(void);
void RMDmotor_config(void);

void RMDmotor_command(uint8_t data[], uint8_t size);

TEST test_RMDmotor_communication_uart(TEST);
TEST test_RMDmotor_drive(TEST);
TEST test_frictionModel(TEST);
TEST test_frictionCompesation(TEST);
TEST test_RMDmotor_sensorlessImpedanceControl(TEST);
TEST test_RMDmotor_sensorlessImpedanceControl_2(TEST);
#endif /* 1_FUNC_FUNC_BLE_HC05_H_ */

