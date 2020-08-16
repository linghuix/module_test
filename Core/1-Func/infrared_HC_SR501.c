/*
 * infrared_HC_SR501.c
 *
 *  Created on: May 29, 2020
 *      Author: test
 */


#include "infrared_HC_SR501.h"

uint8_t infred;


#define IR_PIN GPIOC,GPIO_PIN_15


/*
 * author lhx
 * May 31, 2020
 *
 * @brief : 1 - 表示检测到人体的红外线
 * 			0 - 表示未检测到人体的红外线
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void GetIR (void)
{
	infred = HAL_GPIO_ReadPin(IR_PIN);
}

