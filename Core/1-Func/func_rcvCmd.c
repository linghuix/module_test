/*
 * func_BLE.c
 *
 *  Created on: May 25, 2020
 *      Author: test
 */
#include "func_rcvCmd.h"

//#define CMD_TEST


uint8_t cmd_receivebyte;

void RcvCmd_Init(void)
{
	MX_USART1_UART_Init();
	
	HAL_UART_Receive_IT(&huart1, &cmd_receivebyte, 1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	// 空闲中断
}



#ifdef CMD_TEST

uint8_t CMD_CommandReceive[20], cmd_length =0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		CMD_CommandReceive[cmd_length] = cmd_receivebyte;
		cmd_length++;
		HAL_UART_Receive_IT(&huart1, &cmd_receivebyte, 1);
	}
}

/**
  * @brief:  串口空闲中断回调函数
  * @retval: None
  */
struct parasturct{
	
	int para[50];		// 解读出的整型数字
	uint8_t paranum;	// 解读出的整型数字数量
};


/**
* @brief: 串口输入字符串"5/6/7/8"，解读出所有整数数字。支持正负数字，串口输入需要换行
* @note：对于输入不全的，不会更新对应参数；但是输入过多的，会更新对应参数；输入多少但是全的，会更新。
* “/70” - 不更新  “90/80” 更新 “89/”不更新
* @parameter parasturct:	存储输入数据结构体
* @parameter paradata:		串口输入的命令字符串
* @parameter length:		串口输入的命令字符串字节数
 */

#include<stdlib.h>

TEST inputPara(struct parasturct * parasturct, uint8_t * paradata, uint8_t length)
{
	uint8_t para[10];						// paradata 读取串口输入字符，para 读取数字
	
	int index = 0;							// index 光标
	parasturct->paranum = 0;				// paranum 参数数量
	
	for(int i=0; i<length; i++){
				
		if(paradata[i] == '/'){
			para[index] = '\0';								// para[]字符串为一个数字
			parasturct->para[parasturct->paranum++] = atoi( (char *)para );			// 字符串转整型
			index = 0;
		}
		else if(paradata[i] == '\n'){
			parasturct->para[parasturct->paranum++] = atoi( (char *)para );
			break;
		}
		else{
			para[index++] = paradata[i];
		}
	}
	
	uint8_t output[2];						//串口输出
	for(int i=0;i<parasturct->paranum;i++){

		HAL_UART_Transmit(&huart1, (uint8_t *)&parasturct->para[i], sizeof(output), 500);
		output[0] = 0xFF;
		HAL_UART_Transmit(&huart1, output, 1, 500);
	}
}


/**
  * @brief:  串口空闲中断回调函数
  * @retval: None
  */
float pid_p, pid_i, pid_d;

struct parasturct para = {0};
void inputPara(struct parasturct * parasturct, uint8_t * paradata, uint8_t length);
TEST USAR_UART1_IDLECallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		HAL_UART_Transmit(&huart1, CMD_CommandReceive, cmd_length, 500);
		
		inputPara(&para, CMD_CommandReceive, cmd_length);
		
		int flag=0;                             // 为零表明输入参数没有零的
		for(int i=0;i<para.paranum;i++){
			if(para.para[i] == 0){
				flag++;
			}
		}
		if(flag == 0){
			
			pid_p = (float)para.para[0] / 10;
			pid_i = (float)para.para[1] / 10;
			pid_d = (float)para.para[2] / 10;
		}
		cmd_length = 0;
	}
}

/**
* @brief: 串口空闲中断服务函数
* @note: 	1. need to put into stm32f10x_it.c
* 
* @retval: None
 */
TEST IDLE_UART_IRQHandler (UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   // IDLE 
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                     
            USAR_UART1_IDLECallback(huart);                        
        }
	}
}


/*********************************************************************/
TEST test_RcvCMD_communication(void){
    
    RcvCmd_Init();
}

#endif
