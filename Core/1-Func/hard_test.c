/**************************************************
* @file      hard_test.c
* @author    not wwy
* @version   
* @date      
* @brief     STM32 hardware test 
***************************************************
* @attention
 
***************************************************
*/

//#define HARD_TEST
/* Include ---------------------------------------*/
#include "hard_test.h"

//#define USART3_TEST
//#define USART4_TEST
//#define USART5_TEST

/* Global Variable -------------------------------*/


#ifdef USART1_TEST
/**
  * @brief  test transmit of usart1
  * @note 1. 需要修改 void USART1_IRQHandler(void) 中的内容
  *       2. 必须不存在 printf 这个函数
  * @note This function is declared as __weak to be overwritten in case of other
  * @retval None
  */
	
uint8_t HardwareTestData[7] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77};

TEST test_USART1_communication(void)
{
	MX_USART1_UART_Init();
	
	HAL_UART_Transmit_IT(&huart1, HardwareTestData, 6);
}



/**
  * @brief  test receive of usart1
  * @note 1. 需要修改 void USART1_IRQHandler(void) 中的内容
  *       2. 必须不使用 printf 这个函数
  * @note This function is declared as __weak to be overwritten in case of other
  * @retval None
  */
	
uint8_t hardtest_CommandReceive[20], hardtest_receivebyte, hardtest_length;
TEST test_USART1_Rcv(void)
{
	MX_USART1_UART_Init();
	
	HAL_UART_Receive_IT(&huart1, &hardtest_receivebyte, 1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	// 空闲中断
}


	/**
	  * @brief  串口中断回调函数
	  * @retval None
	  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{

//	if(huart->Instance == USART1){
//		hardtest_CommandReceive[hardtest_length] = hardtest_receivebyte;
//		hardtest_length++;
//		HAL_UART_Receive_IT(&huart1, &hardtest_receivebyte, 1);
//	}
//}

	/**
	  * @brief  串口空闲中断回调函数
	  * @retval None
	  */
struct parasturct{
	
	int para[50];		// 解读出的整型数字
	uint8_t paranum;	// 解读出的整型数字数量
};

TEST inputPara(struct parasturct * parasturct, uint8_t * paradata, uint8_t length);
TEST USAR_UART1_IDLECallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		struct parasturct para = {0};
		
		HAL_UART_Transmit(&huart1, hardtest_CommandReceive, hardtest_length, 500);
		
		inputPara(&para, hardtest_CommandReceive, hardtest_length);
		
		hardtest_length = 0;
	}
}


		/**
		* @brief: 串口输入字符串"5/6/7/8"，解读出所有整数数字 
		* @note
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
			  * @brief 
			  * @note 	1. need to put into stm32f10x_it.c
			  * 
			  * @retval None
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

#endif

#ifdef USART3_TEST

int u3_length=0;
uint8_t u3_receivebyte;
/**
  * @brief  test transmit of usart2
  * @note 
  * @note 
  * @retval 
  */
	
uint8_t HardwareTestData[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x66,0x55,0x44}, u3_CommandReceive[20]={0};

TEST test_USART3_communication(void)
{
	MX_USART3_UART_Init();
	
	HAL_UART_Transmit_IT(&huart3, HardwareTestData, 10);
	HAL_UART_Receive_IT(&huart3, &u3_receivebyte, 1);
}


TEST HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance == USART3){
		TEST_MSG("U3rcv[%d] ", u3_length);
		TEST_MSG("0x%x\t", u3_receivebyte);
//		u3_CommandReceive[u3_length] = u3_receivebyte;
		u3_length++;
		HAL_UART_Receive_IT(&huart3, &u3_receivebyte, 1);
	}
	
}
#endif

#ifdef USART4_TEST

int u3_length=0;
uint8_t u3_receivebyte;
/**
  * @brief  test transmit of usart2
  * @note 
  * @note 
  * @retval 
  */
	
uint8_t HardwareTestData[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x66,0x55,0x44}, u3_CommandReceive[20]={0};

TEST test_UART4_communication(void)
{
	MX_UART4_UART_Init();
	
	HAL_UART_Transmit_IT(&huart4, HardwareTestData, 10);
	HAL_UART_Receive_IT(&huart4, &u3_receivebyte, 1);
}


TEST HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance == UART4){
		TEST_MSG("U3rcv[%d] ", u3_length);
		TEST_MSG("0x%x\t", u3_receivebyte);
//		u3_CommandReceive[u3_length] = u3_receivebyte;
		u3_length++;
		HAL_UART_Receive_IT(&huart3, &u3_receivebyte, 1);
	}
	
}
#endif

#ifdef USART5_TEST

int u3_length=0;
uint8_t u3_receivebyte;
/**
  * @brief  test transmit of usart2
  * @note 
  * @note 
  * @retval 
  */
	
uint8_t HardwareTestData[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x66,0x55,0x44}, u3_CommandReceive[20]={0};

TEST test_UART5_communication(void)
{
	MX_UART5_UART_Init();
	
	HAL_UART_Transmit_IT(&huart5, HardwareTestData, 10);
	HAL_UART_Receive_IT(&huart5, &u3_receivebyte, 1);
}


TEST HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance == UART5){
		TEST_MSG("U3rcv[%d] ", u3_length);
		TEST_MSG("0x%x\t", u3_receivebyte);
//		u3_CommandReceive[u3_length] = u3_receivebyte;
		u3_length++;
		HAL_UART_Receive_IT(&huart3, &u3_receivebyte, 1);
	}
	
}
#endif

void hard_test(void)
{
//	test_UART4_communication();
//	test_UART5_communication();
}

