
#include "conf_usart.h"
//#include "conf_global.h""


/* gobal variable*/
GPIO_InitTypeDef GPIO_InitStruct;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

DMA_HandleTypeDef hdma_usart1_rx;



/**
  * @brief USART1 Function Initialization
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void)
{
    MSG_BSTART("usart1","conf");
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
    MSG_ASTART("usart1","conf");
}



/**
  * @author lhx  May 13, 2020
  * @brief  usart hardware initalization      
  * 		引脚初始化, 时钟开启
  * @calledIn HAL_UART_MspInit()
  * @param  uartHandle: UART Handler
  * @retval None
  */
void USART_Hardware_Init(UART_HandleTypeDef* uartHandle)
{
    if(uartHandle->Instance==USART1)
    {
		/*
		UART1 TX —— PA9
		UART1 RX —— PA10
		*/
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
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
}



/**
  * @author lhx  May 13, 2020
  * @brief  串口中断功能设置(注意此处并没有激活/启用中断功能)
  * @param  uartHandle: UART Handler
  * @retval None
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
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		//__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
    }
    else if(uartHandle->Instance==USART3)
    {
		/* Peripheral interrupt init */
		HAL_NVIC_SetPriority(USART3_IRQn, 0, 2);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}




/**
  * @brief USART2 Function Initialization
  * @param None
  * @retval None
  */
void MX_USART2_UART_Init(void)
{
  MSG_BSTART("usart2","conf");
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

  MSG_ASTART("usart2","conf");
}




/**
  * @brief USART3 Function Initialization
  * @param None
  * @retval None
  */
void MX_USART3_UART_Init(void)
{
MSG_BSTART("usart3","conf");
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
MSG_ASTART("usart3","conf");
}




/**
  * @author lhx  May 13, 2020
  * @brief  USART MCU Specific Package Initialization
  * @calledIn HAL_UART_Init()，用于硬件初始化
  * @call   USART_Hardware_Init()
  *         USART_NVIC_Init()
  * @param  uartHandle: UART Handler
  * @retval None
  */ 
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	USART_Hardware_Init(uartHandle);
	USART_NVIC_Init(uartHandle);
}




/**
  * @author lhx  May 13, 2020
  * @brief  USART MCU Specific Package De-Initialization
  * @param  uartHandle: UART Handler
  * @retval None
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
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD6     ------> USART2_RX
    PD5     ------> USART2_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6|GPIO_PIN_5);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART2_IRQn);

  }
  else if(uartHandle->Instance==USART3)
  {

    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PD9     ------> USART3_RX
    PD8     ------> USART3_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9|GPIO_PIN_8);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART3_IRQn);

  }
} 


/*void USART_init(void)*/
/*{*/
/*	MX_USART1_UART_Init();*/
/*	MX_USART2_UART_Init();*/
/*}*/


