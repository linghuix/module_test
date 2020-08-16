/*
 * Servos.c
 *
 *  Created on: Apr 12, 2020
 *      Author: test
 */
#include "Servos.h"

//#define SERVO_TEST

#define Servo_TIM TIM4

void Servo_Init(void)
{
	MX_TIM_PWMOUT(Servo_TIM, 10000, 200);
}

void Servo_Start(void)
{
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

/*
 * author lhx
 * Apr 12, 2020
 *
 * @brief :
 * 1. PWM输出测试代码，用Jlink示波器检测变量flag，
 * 2. 如果flag的波形是设置的PWM周期的话，则代码可以正常运行
 * Window > Preferences > C/C++ > Editor > Templates.
 */
#ifdef SERVO_TEST
TEST Test_Servos(void)
{
	Servo_Init();
	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
}

uint8_t flag=0;
TEST Servo_PWMCallback(void)
{
	flag = !flag;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	Servo_PWMCallback();
}
#endif
