
/* Includes ------------------------------------------------------------------*/
#include "conf_usart.h"

GPIO_InitTypeDef GPIO_InitStruct;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;

DMA_HandleTypeDef hdma_usart1_rx;


/**
 * @author lhx
 * @Date May 13, 2020
 *
 * @brief :USART init function
 * 			配置相关的波特率等参数
 */
void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&huart1) != HAL_OK){
    Error_Handler()
  }
}
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_MultiProcessor_Init(&huart2, 0, UART_WAKEUPMETHOD_IDLELINE) != HAL_OK)
  {
    Error_Handler()
  }

}
void MX_USART3_UART_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_MultiProcessor_Init(&huart3, 0, UART_WAKEUPMETHOD_IDLELINE) != HAL_OK)
  {
    Error_Handler()
  }
}
void MX_UART4_UART_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&huart4) != HAL_OK){
    Error_Handler()
  }
}
void MX_UART5_UART_Init(void)
{
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&huart5) != HAL_OK){
    Error_Handler()
  }
}


/**
 * @author lhx
 * @Date May 13, 2020
 *
 * @brief : 调用自HAL_UART_MspInit
 * 			串口相关引脚初始化
 * 			串口和引脚时钟开启
 */
void USART_Hardware_Init(UART_HandleTypeDef* uartHandle)
{
    if(uartHandle->Instance==USART1)
    {
		/*
		UART1 TX - PA9
		UART1 RX - PA10
		*/
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }
    else if(uartHandle->Instance==USART2)
    {

      /* Peripheral clock enable */
      __HAL_RCC_USART2_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();

      /**USART2 GPIO Configuration
      PA3     ------> USART2_RX
      PA2     ------> USART2_TX
      */
			GPIO_InitStruct.Pin = GPIO_PIN_2;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			GPIO_InitStruct.Pin = GPIO_PIN_3;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    else if(uartHandle->Instance==USART3)
    {
      /* Peripheral clock enable */
      __HAL_RCC_USART3_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();

      /**USART3 GPIO Configuration
      PB11     ------> USART3_RX
      PB10     ------> USART3_TX
      */

			GPIO_InitStruct.Pin = GPIO_PIN_10;
			GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

			GPIO_InitStruct.Pin = GPIO_PIN_11;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }

    else if(uartHandle->Instance==UART4)
    {

		/* Peripheral clock enable */
		__HAL_RCC_UART4_CLK_ENABLE();
	  
		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**UART4 GPIO Configuration    
		PC10     ------> UART4_TX
		PC11     ------> UART4_RX 
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    }
	
    else if(uartHandle->Instance==UART5)
    {
		/* Peripheral clock enable */
		__HAL_RCC_UART5_CLK_ENABLE();
	  
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		
		/**UART5 GPIO Configuration    
			PC12     ------> UART5_TX
			PD2     ------> UART5_RX 
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    }
}

/**
 * @author lhx
 * @Date May 13, 2020
 *
 * @brief : 串口中断功能使能,优先级设置，注意中断在此处未开启
 * 
 */
void USART_NVIC_Init(UART_HandleTypeDef* uartHandle)
{
    if(uartHandle->Instance==USART1)
    {
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);

    }
    else if(uartHandle->Instance==USART2)
    {
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    else if(uartHandle->Instance==USART3)
    {
		/* Peripheral interrupt init */
		HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
    else if(uartHandle->Instance==UART4)
    {
		/* UART4 interrupt Init */
		HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
    }
    else if(uartHandle->Instance==UART5)
    {
		/* UART5 interrupt Init */
		HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(UART5_IRQn);
    }
}

/**
 * @author lhx
 * @Date May 13, 2020
 *
 * @brief : 调用自 HAL_UART_Init，用于硬件初始化
 * 
 */
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	USART_Hardware_Init(uartHandle);
	USART_NVIC_Init(uartHandle);
}

/**
 * @author lhx
 * @Date May 13, 2020
 *
 * @brief : 串口硬件去初始化
 * 
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PB7     ------> USART1_RX
    PB6     ------> USART1_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7|GPIO_PIN_6);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
  else if(uartHandle->Instance==USART2)
  {
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD6     ------> USART2_RX
    PD5     ------> USART2_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6|GPIO_PIN_5);

    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
  else if(uartHandle->Instance==USART3)
  {
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PD9     ------> USART3_RX
    PD8     ------> USART3_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9|GPIO_PIN_8);

    HAL_NVIC_DisableIRQ(USART3_IRQn);
  }
  else if(uartHandle->Instance==UART4)
  {
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    HAL_NVIC_DisableIRQ(UART4_IRQn);
  }
  else if(uartHandle->Instance==UART4)
  {
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    HAL_NVIC_DisableIRQ(UART4_IRQn);
  }
} 


/**
 * @author lhx
 * @Date May 13, 2020
 *
 * @brief : 串口发送回调函数
 * 
 */
TEST HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2){
		TEST_MSG("Tx2C\r\n");	// usart2 tx send completely
	}
	if(huart->Instance == USART3){
		TEST_MSG("Tx3C\r\n");
	}
	if(huart->Instance == UART4){
		TEST_MSG("Tx4C\r\n");
	}
	if(huart->Instance == UART5){
		TEST_MSG("Tx5C\r\n");
	}
}

/**
 * @author lhx
 * @Date 01/17, 2022
 *
 * @brief : USART1 IDLE interrupt service function CallBack
            串口一空闲中断回调函数
		在 IDLE_UARTX_IRQHandler 中被调用
		在其他func或者task函数中可替换此函数
 */
__weak void USART1_IDLE_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US1_IDLE\r\n");
}
__weak void USART2_IDLE_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US2_IDLE\r\n");
}
__weak void USART3_IDLE_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US3_IDLE\r\n");
}
__weak void USART4_IDLE_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US4_IDLE\r\n");
}
__weak void USART5_IDLE_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US5_IDLE\r\n");
}

/**
 * @author lhx
 * @Date 01/17, 2022
 *
 * @brief : USART1 RX interrupt service function CallBack
            串口一空闲中断回调函数
		在其他func或者task函数中可替换此函数
 */
__weak void USART1_RX_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US1_RX\r\n");
}
__weak void USART2_RX_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US2_RX\r\n");
}
__weak void USART3_RX_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US3_RX\r\n");
}
__weak void USART4_RX_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US4_RX\r\n");
}
__weak void USART5_RX_CallBack_Conf_usart_c(void)
{
	TEST_MSG("US5_RX\r\n");
}


/**
  * @brief  callback function for USARTX
  * @param  huart poiner to UART structure
  * @retval HAL status
  */
TEST HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance == USART1){
		USART1_RX_CallBack_Conf_usart_c();
//		TEST_MSG("Usart2 rcv %d\r\n", length);
//		// TEST_MSG("0x%x\t", receivebyte);
//		CommandReceive[length] = receivebyte;
//		length++;
	}
	if(huart->Instance == USART2){
		USART2_RX_CallBack_Conf_usart_c();
	}
	if(huart->Instance == USART3){
		USART3_RX_CallBack_Conf_usart_c();
	}
	if(huart->Instance == UART4){
		USART4_RX_CallBack_Conf_usart_c();
	}
	if(huart->Instance == UART5){
		USART5_RX_CallBack_Conf_usart_c();
	}
}
/**
 * @author lhx
 * @Date 01/17, 2022
 *
 * @brief : USARTX IDLE interrupt Handler
            串口空闲中断处理函数
		is called in UARTX_IRQHandler
 */
TEST IDLE_UART1_IRQHandler (UART_HandleTypeDef *huart)
{
		if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   // IDLE ?
		{
				__HAL_UART_CLEAR_IDLEFLAG(huart);
				USART1_IDLE_CallBack_Conf_usart_c();
		}
}
TEST IDLE_UART2_IRQHandler (UART_HandleTypeDef *huart)
{
		if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   // IDLE ?
		{
				__HAL_UART_CLEAR_IDLEFLAG(huart);
				USART2_IDLE_CallBack_Conf_usart_c();
		}
}
TEST IDLE_UART3_IRQHandler (UART_HandleTypeDef *huart)
{
		if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   // IDLE ?
		{
				__HAL_UART_CLEAR_IDLEFLAG(huart);
				USART3_IDLE_CallBack_Conf_usart_c();
		}
}
TEST IDLE_UART4_IRQHandler (UART_HandleTypeDef *huart)
{
		if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   // IDLE ?
		{
				__HAL_UART_CLEAR_IDLEFLAG(huart);
				USART4_IDLE_CallBack_Conf_usart_c();
		}
}
TEST IDLE_UART5_IRQHandler (UART_HandleTypeDef *huart)
{
		if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   // IDLE ?
		{
				__HAL_UART_CLEAR_IDLEFLAG(huart);
				USART5_IDLE_CallBack_Conf_usart_c();
		}
}



/**
 * @author lhx
 * @Date 01/17, 2022
 *
 * @brief : USART2-5 global interrupt Handler
            串口全局中断处理函数
 */
void USART2_IRQHandler(void)
{
	printf("u2 irq\r\n");
	HAL_UART_IRQHandler(&huart2);	
	IDLE_UART2_IRQHandler(&huart2);
}
void USART3_IRQHandler(void)
{
	printf("u3 irq\r\n");
	HAL_UART_IRQHandler(&huart3);
	IDLE_UART3_IRQHandler(&huart3);
}
void UART4_IRQHandler(void)
{
	printf("u4 irq\r\n");
	HAL_UART_IRQHandler(&huart4);
	IDLE_UART4_IRQHandler(&huart4);
}
void UART5_IRQHandler(void)
{
	printf("u5 irq\r\n");
	HAL_UART_IRQHandler(&huart5);
	IDLE_UART5_IRQHandler(&huart5);
}

/**
  ******************************************************************************
  * @section    Test
  * @author  xlh
  * @brief   
  ******************************************************************************
  */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TEST @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

void USART_init(void)
{
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
