/**
 * func_BLE.c
 *
 *  Created on: May 25, 2020
 *      Author: test
 *
 *  use USART 1 RX function
 *  @brief: 实现上位机通过蓝牙(无线/有线)串口控制单片机
			本文实现硬件的初始化和 Command 的解析
 */

#include "func_BLE_HC05.h"

#define COMMANDLENGTH 200		//bytes

/**
 * @author lhx
 * @Date 01/17, 2020
 *
 * @brief : BLE Command receiver Initalization
 */
uint8_t BLE_CommandReceive[COMMANDLENGTH], BLE_receivebyte, BLE_length;
void HC05_RcvCmd_Init(void)
{
	MX_USART1_UART_Init();
}

void HC05_RcvCmd_Start(void)
{
	HAL_UART_Receive_IT(&huart1, &BLE_receivebyte, 1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	// 空闲中断
}

/**
 * @author lhx
 * @Date 01/17, 2020
 *
 * @brief : USART1_IDLE_CallBack. set flag to notice 
 */
int idleflag = 0;
void USART1_IDLE_CallBack_Conf_usart_c(void)
{
	idleflag = 1;
}

/**
 * @author lhx
 * @Date 01/17, 2020
 *
 * @brief : USART1_RX_CallBack.  BLE_CommandReceive 中的串口数据接收
 * @retval None
call inputPara()

 */
void USART1_RX_CallBack_Conf_usart_c(void)
{
	TEST_MSG("get cmad\r\n");
	BLE_CommandReceive[BLE_length] = BLE_receivebyte;
	BLE_length++;
	HAL_UART_Receive_IT(&huart1, &BLE_receivebyte, 1);
}

/**
  ******************************************************************************
  * @section    Test
  * @author  xlh
  * @brief   
  ******************************************************************************
  */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TEST @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


//#define BLE_CMD_TEST
#ifdef BLE_CMD_TEST

/**
  * @brief  串口空闲中断回调函数
  * @retval None
  */
TEST test_CommandReceiver(TEST)
{
	HC05_RcvCmd_Init();
	HC05_RcvCmd_Start();
	
	TEST_MSG("please input 11 int split by '/'. the first int must be 170. input string must end with Enter\r\n");
	TEST_MSG("for example: \"170/5/6/9/4/4/5/5/8/9/7\\r\\n\"\r\n");
	
	/* idle 串口数据解析 */
	while(1) {
		if( idleflag == 1) {
			commandPrase();
			idleflag = 0;
		}
	}
}


/**
  * @brief  存储命令解析结果的结构体,最多能够存储50个int数据
  */
struct parasturct
{
	int para[50];		// 解读出的整型数字上限
	uint8_t paranum;	// 解读出的整型数字数量
};
struct parasturct para = {0};


/**
  * @brief  BLE_CommandReceive 中的串口数据解析
  * @retval None
call inputPara()
  */
void inputPara(struct parasturct * parasturct, uint8_t * paradata, uint8_t length);
void commandPrase(void)
{
	TEST_MSG("Received command: %s",BLE_CommandReceive );
	TEST_MSG("Received command length: %d\r\n", BLE_length );
	
	inputPara(&para, BLE_CommandReceive, BLE_length);
	
	int flag=1;						// 判断第一个数是否为170
	if( para.para[0] == 170 ){		// 第一个数字必为170 二进制位 0x10101010
		flag = 0;
	}
	if(flag == 0){
		printf("get number int %d\r\n", para.paranum);
		if ( para.paranum >= 11){
			/* assistive torque curve*/
			int PREDICT_TIME;
			float tao_Ep,fai_Ep,fai_Er,fai_Ef,a[3],b[3];
			tao_Ep = (float)para.para[1]/100.0;		// 5-10 Nm
			fai_Ep = (float)para.para[2]/1000.0; 	// 0.2-0.3
			fai_Er = (float)para.para[3]/1000.0;	// 0.1-0.2
			fai_Ef = (float)para.para[4]/1000.0;	// 0.1-0.2
			
			PREDICT_TIME = para.para[5];
			
			a[0] = (float)para.para[6]/1000.0;
			a[1] = (float)para.para[7]/1000.0;
			a[2] = (float)para.para[8]/1000.0;
			b[0] = (float)para.para[9]/1000.0;
			b[1] = (float)para.para[10]/1000.0;
			b[2] = (float)para.para[11]/1000.0;
			
			TEST_MSG("para change : PREDICT_TIME=%d\r\n", PREDICT_TIME);
			TEST_MSG("para change : [tao_Ep,fai_Ep,fai_Er,fai_Ef]=[%.3f,%.3f,%.3f,%.3f]\r\n", tao_Ep,fai_Ep,fai_Er,fai_Ef);
			TEST_MSG("para change : [a,b]=[[%.3f,%.3f,%.3f],[%.3f,%.3f,%.3f]]\r\n", a[0],a[1],a[2],b[0],b[1],b[2]);
			printf("\r\n#####\r\n");
		}
	}
	BLE_length = 0;
}

/**
	* @brief: 串口输入字符串"5/6/7/8\r\n"，解读出所有整数数字的算法。支持正负数字，串口输入需要换行
	* @note ：对于输入不全的，不会更新对应参数；但是输入过多的，会更新对应参数；输入过少但是全的，会更新。
	* “/70” - 不更新  “90/80” 更新 “89/”不更新
	* @parameter parasturct:	存储输入数据结构体
	* @parameter paradata:		串口输入的命令字符串
	* @parameter length:		串口输入的命令字符串字节数
	* @Note input string must end with Enter
is called in commandPrase()
 */
#include<stdlib.h>
TEST inputPara(struct parasturct * parasturct, uint8_t * paradata, uint8_t length)
{
	uint8_t para[50];						// paradata 读取串口输入字符，para 读取数字
	
	int index = 0;							// index 光标
	parasturct->paranum = 0;				// paranum 参数数量
	
	for(int i=0; i<length; i++){
				
		if(paradata[i] == '/'){
			para[index] = '\0';								// para[]字符串为一个数字
			parasturct->para[parasturct->paranum++] = atoi( (char *)para );			// 字符串转整型
			index = 0;
		}
		else if(paradata[i] == '\n'){
			para[index] = '\0';	
			parasturct->para[parasturct->paranum++] = atoi( (char *)para );
			break;
		}
		else{
			para[index++] = paradata[i];
		}
	}
	
//	uint8_t output[2];						//串口输出
//	for(int i=0;i<parasturct->paranum;i++){

//		HAL_UART_Transmit(&huart1, (uint8_t *)&parasturct->para[i], sizeof(output), 500);
//		output[0] = 0xFF;
//		HAL_UART_Transmit(&huart1, output, 1, 500);
//	}
}

#endif
