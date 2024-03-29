/*
 * accelerate.c
 *
 *  Created on: May 25, 2020
 *      Author: test
 */
#include "func_accelerate.h"

//#define ACC_TEST


uint8_t imu_2_flag = 0;
uint8_t flag_1,flag_2,flag_3;
uint8_t flag_11,flag_22,flag_33;
uint8_t acc2[11];
int16_t a2[3], w2[3], angle2[3];
int16_t a1[3], w1[3], angle1[3];
float T;

uint8_t acc1[11];


void Acc1_Init(void)
{
	MX_USART3_UART_Init();
}


void Acc2_Init(void)
{
	MX_USART2_UART_Init();
}

uint8_t test_data[5] = {0x55,0x00,0x00,0x00,0x00};

void Acc2_Start(void)
{
	//HAL_UART_Transmit_IT(&acc2_huart, test_data, 5);
	HAL_UART_Receive_IT(&acc2_huart, acc2, 1);
}



void Acc1_Start(void)
{
	//HAL_UART_Transmit_IT(&acc1_huart, test_data, 5);
	HAL_UART_Receive_IT(&acc1_huart, acc1, 1);
}

#ifdef ACC_TEST

TEST acc_test(void){
	
	 Acc1_Init(); 
	 Acc1_Start();
	
}

uint8_t state1 = 0;
uint8_t state2 = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		//HAL_UART_Receive_IT(&huart1, acc1, 6);
		HAL_UART_Transmit_IT(&huart1, test_data, 5);
	}
	if(huart->Instance == acc1_uart){
	    //imu_2_flag = 1;
		switch(state1){
			case 0:
				if(acc1[0]==0x55){state1 = 1;HAL_UART_Receive_IT(&acc1_huart, &acc1[1], 10);}
				else HAL_UART_Receive_IT(&acc1_huart, acc1, 1);
				break;
			case 1:
				switch(acc1[1]){
				  case 0x51:
					a1[0] = ((int16_t)(acc1[3]<<8| acc1[2]));
					a1[1] = ((int16_t)(acc1[5]<<8| acc1[4]));
					a1[2] = ((int16_t)(acc1[7]<<8| acc1[6]));
					T = ((int16_t)(acc1[9]<<8| acc1[8]));
					flag_1 = 1;
					break;
				  case 0x52:
					w1[0] = ((int16_t)(acc1[3]<<8| acc1[2]));
					w1[1] = ((int16_t)(acc1[5]<<8| acc1[4]));//hip flex z
					w1[2] = ((int16_t)(acc1[7]<<8| acc1[6]));
					T = ((int16_t)(acc1[9]<<8| acc1[8]));
					flag_2 = 1;
					break;
				  case 0x53:
					angle1[0] = ((int16_t)(acc1[3]<<8| acc1[2]));
					angle1[1] = ((int16_t)(acc1[5]<<8| acc1[4]));//hip felx
					angle1[2] = ((int16_t)(acc1[7]<<8| acc1[6]));
					T = ((int16_t)(acc1[9]<<8| acc1[8]));
					flag_3 = 1;
					break;
				}
				state1 = 0;HAL_UART_Receive_IT(&acc1_huart, acc1, 1);
				break;
		}
	}
	if(huart->Instance == acc2_uart){
		switch(state2){
			case 0:
				if(acc2[0]==0x55){state2 = 1;HAL_UART_Receive_IT(&acc2_huart, &acc2[1], 10);}
				else HAL_UART_Receive_IT(&acc2_huart, acc2, 1);
				break;
			case 1:
				switch(acc2[1]){
				  case 0x51:
					a2[0] = ((int16_t)(acc2[3]<<8| acc2[2]));
					a2[1] = ((int16_t)(acc2[5]<<8| acc2[4]));
					a2[2] = ((int16_t)(acc2[7]<<8| acc2[6]));
					T = ((int16_t)(acc2[9]<<8| acc2[8]));
					flag_11 = 1;
					break;
				  case 0x52:
					w2[0] = ((int16_t)(acc2[3]<<8| acc2[2]));
					w2[1] = ((int16_t)(acc2[5]<<8| acc2[4]));//hip flex z
					w2[2] = ((int16_t)(acc2[7]<<8| acc2[6]));
					T = ((int16_t)(acc2[9]<<8| acc2[8]));
					flag_22 = 1;
					break;
				  case 0x53:
					angle2[0] = ((int16_t)(acc2[3]<<8| acc2[2]));
					angle2[1] = ((int16_t)(acc2[5]<<8| acc2[4]));//hip felx  -90~90
					angle2[2] = ((int16_t)(acc2[7]<<8| acc2[6]));
					T = ((int16_t)(acc2[9]<<8| acc2[8]));
					flag_33 = 1;
					break;
				}
				state2 = 0;HAL_UART_Receive_IT(&acc2_huart, acc2, 1);
				break;
		}
	}
}






//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance == USART1){
//	    //imu_2_flag = 1;
//		HAL_UART_Receive_IT(&huart1, acc1, 6);
//		HAL_UART_Transmit_IT(&huart1, acc1, 6);
//	}
//	if(huart->Instance == acc1_uart){
//	    //imu_2_flag = 1;
//		HAL_UART_Receive_IT(&acc1_huart, acc1, 6);
//		HAL_UART_Transmit_IT(&acc1_huart, acc1, 6);
//	}
//	if(huart->Instance == acc2_uart){
//	    //imu_2_flag = 1;
//		HAL_UART_Receive_IT(&acc2_huart, acc2, 6);
//		HAL_UART_Transmit_IT(&acc2_huart, acc2, 6);
//	}
//}


/*
 * author lhx
 * May 25, 2020
 *
 * @brief : 通讯测试
 * 			对于stm32f103rbt6,未开启AFIO，映射。
 * 			串口２　-　PA2-RX  PA3-TX
 * 			串口３　-  PB10-TX  PB11-RX
 * Window > Preferences > C/C++ > Editor > Templates.
 */

TEST test_acc_communication(void)
{
	Acc1_Init();
	Acc2_Init();

	uint8_t test_data[5] = {0x55,0x66,0x22,0x33,0x44};
	HAL_UART_Transmit_IT(&acc1_huart, test_data, 1);
	HAL_UART_Transmit_IT(&acc2_huart, test_data, 5);
}

/*代码启动*/
float hip1_w, hip1_d, I1;
float hip2_w, hip2_d, I2;
float hip1_raww, hip1_rawd;
float hip2_raww, hip2_rawd;

// Jscope 调试
int debug_hip1_d, debug_hip1_rawd, debug_hip2_d, debug_hip2_rawd;
TEST test_acc_collect(void)
{
//	Acc1_Init();
	Acc2_Init();
//	Acc1_Start();
	Acc2_Start();
	
	while (1){

		/* 左髋关节 加速度信号采集  采样周期约100Hz以上 */
//		if(flag_1 ==1&&flag_2 == 1&&flag_3 == 1){
//			flag_1=0;flag_2=0;flag_3=0;
//			hip1_rawd = angle1[1]/32768.0*180;	hip1_raww = w1[2]/32768.0*2000;
//		}

		/* 右髋关节 加速度信号采集  采样周期约100Hz以上 */
		if(flag_11 ==1&&flag_22 == 1&&flag_33 == 1){
			flag_11=0;flag_22=0;flag_33=0;
			hip2_rawd = angle2[1]/32768.0*180;	hip2_raww = w2[1]/32768.0*2000;
		}
		
//		debug_hip1_rawd = (int)hip1_rawd;
		debug_hip2_rawd = 1000*hip2_rawd;
	}
}
#endif
