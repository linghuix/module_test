

/*
 * ultrasonic_sensor.c
 *
 *  Created on: Apr 5, 2020
 *      Author: test
 */
#include "ultrasonic_sensor.h"

//#define ULTRA_TEST

#define Trigger		GPIOB,GPIO_PIN_1
#define TIM_ultrasonic TIM1
#define CaptureChannel TIM_CHANNEL_3

void Ultrasonic_Init(void);
void Ultrasonic_Start(void);
void start_getDistance(void);

/*
 * author lhx
 * Apr 5, 2020
 *
 * @brief :
 * 1.设置定时器为输入上升沿捕获, channel3通道 PA10
 * 2.设置引脚Trigger为输出
 * 3.Trigger引脚初始化为低电平
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void Ultrasonic_Init(void)
{
	MX_GPIO_output_Init();
	HAL_GPIO_WritePin(Trigger, GPIO_PIN_RESET);
	MX_TIM_CaptureInterrupt(TIM_ultrasonic, 10000, 20000);
}


void Ultrasonic_Start(void)
{
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);
	int i = 0;
	//more than 10us high signal.
	HAL_GPIO_WritePin(Trigger, GPIO_PIN_SET);
	while(i<90) i++;
	HAL_GPIO_WritePin(Trigger, GPIO_PIN_RESET);
}

void start_getDistance(void)
{
	Ultrasonic_Start();
}

/*
 * author lhx
 * Apr 6, 2020
 *
 * @brief : 捕获中断服务函数
 * 1. 测量高电平的时间
 * 2. 检测完成一组后，再次启动捕获
 * Window > Preferences > C/C++ > Editor > Templates.
 */
uint8_t inputCapture_flag = 1;
void ultraCapture_callback(TIM_HandleTypeDef *htim)
{
	uint32_t distance,hightime;

	if(inputCapture_flag == 0){				//下降沿，结束一次测距,　并重启测距
		MSG_deviceTest("Capture falling\n");
		inputCapture_flag = 1;
		hightime = getCaptureCount(TIM_ultrasonic,TIM_CHANNEL_3);
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
//		MSG_deviceTest("distance: %d\n",distance/10.0/2*34);		//cm
		distance = hightime;
		MSG_deviceTest("distance: %d time %dx0.1ms\n", distance, hightime);
		start_getDistance();
	}
	else{									//上升沿，开始测距
		MSG_deviceTest("Capture rising\n");
		setTimeCount(TIM_ultrasonic, 0);
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
		inputCapture_flag = 0;
	}
}

/*
 * author lhx
 * Apr 12, 2020
 *
 * @brief : 溢出中断
 * 1. 捕获超时(高电平太高时，此时超声波测量距离太远了，精度不足，舍弃)，重新启动捕获上升沿
 * Window > Preferences > C/C++ > Editor > Templates.
 */
void Period_callback(TIM_HandleTypeDef *htim)
{
	MSG_deviceTest("timer elapsed restart\n");
	__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
	start_getDistance();
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim1.Instance == TIM_ultrasonic){
//		test_ultraCapture_callback(htim);
		ultraCapture_callback(htim);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim1.Instance == TIM_ultrasonic)
		Period_callback(htim);
}


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#ifdef ULTRA_TEST
/*
 * author lhx
 * Apr 6, 2020
 *
 * @brief :
 * 1. 测试超声波传感器是否有效，能够反应距离的大小
 * 2. 检测高电平的检测是否准确，可输入一组PWM波，来检测其高电平持续时间
 * Window > Preferences > C/C++ > Editor > Templates.
 */
void test_ultrasonic(void)
{
	Ultrasonic_Init();
	start_getDistance();
}

/*
 * author lhx
 * Apr 6, 2020
 *
 * @brief : test whether interrupt can capture the rising edge
 * Window > Preferences > C/C++ > Editor > Templates.
 */
int test_capture = 0;
/*1*/
void test_Start(void)
{
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);
	int i = 0;
	test_capture = 1;
	HAL_GPIO_WritePin(Trigger, GPIO_PIN_SET);
	while(i<90) i++;
	test_capture = 0;
	HAL_GPIO_WritePin(Trigger, GPIO_PIN_RESET);
	while(i<90) i++;
	test_capture = 1;
	HAL_GPIO_WritePin(Trigger, GPIO_PIN_SET);
	while(i<90) i++;
	test_capture = 0;
	HAL_GPIO_WritePin(Trigger, GPIO_PIN_RESET);
	while(i<90) i++;
}
/*2*/
void test_ultraCapture_callback(TIM_HandleTypeDef *htim)
{
		MSG_deviceTest("Capture %d\n",test_capture);
		setTimeCount(TIM_ultrasonic, 0);
		inputCapture_flag = 0;
}
/*3*/
void test_Input_capture(void)
{
	Ultrasonic_Init();
	test_Start();
}
#endif
