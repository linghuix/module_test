/**
 * @Servos.c
 * @Created firstly on: Apr 12, 2020
 * @Author: lhx
 
 * @brief 舵机控制
 
 */

#include "Servos.h"
#include "conf_gpio.h"
#define SERVO_TEST

#define Servo_TIM TIM4
#define Servo_HTIM htim4

/**
 * @author lhx
 * @time Sep 16, 2021
 * @brief : 舵机的控制模块、定时器PWM模块初始化；
 * 示例，PWM波初始化为定时器计数频率 100000Hz，定时器最大计数值1000，
		 因此定时器溢出事件发生频率50Hz。选择哪些通道打开，在函数内部设置
 */
void Servo_Init(void)
{
	MX_TIM_PWMOUT(Servo_TIM, 100*1000, 1000);	// 50Hz
}

/**
 * @author lhx
 * @time Sep 16, 2021
 * @brief : 定时器PWM模块TIM_CHANNEL_1开启，即可输出PWM波；
 */
void Servo_Start(void)
{
	HAL_TIM_PWM_Start(&Servo_HTIM, TIM_CHANNEL_1);
}

/**
 * @author lhx
 * @time Sep 16, 2021
 * @brief : 控制舵机的到指定角度；
 * 示例，0.5ms的高电平对应0度，2.5ms高电平对应300度
 * @angle - 舵机角度，单位：度
 */
void setAngle(float angle)
{
	SetTIMCCR(Servo_TIM, TIM_CHANNEL_2, (angle/300.0*2.0+0.5)/10.0*1000.0);	//高电平 0.5~2.5 ms 控制 角度0~300
}
	
#ifdef SERVO_TEST
#define SERVO_ARR    (TIM4->ARR)
/**
 * @author lhx
 * @time Apr 12, 2020
 *
 * @brief :
 * 示例 ： 控制舵机 500*5ms 内从 0度跑到90度
 */
TEST Test_Servos(void)
{
	Servo_Init();
	Servo_Start();
	while(1){
		for(int i=0;i<500;i++){
			setAngle(0+(90-0)/500.0*i);		//500*5ms 内从 0度跑到90度
			HAL_Delay(5);
		}
	}
}

/**
 * @author lhx
 * @time Apr 12, 2020
 *
 * @brief :
 * 1. PWM输出测试代码，用Jlink示波器检测变量flag，
 * 2. 如果flag的波形是设置的PWM周期的话，则代码可以正常运行
 */
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
