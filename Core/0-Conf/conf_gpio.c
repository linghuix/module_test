/**
  ******************************************************************************
  * @file    delay.c
  * @author  Linghui Xu
  * @version V1.0
  * @date    2019.4.27
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  * 1. 本程序主要存放引脚的配置，针对GPIO以及其他复用功能TIME，ADC，USART，CAN...
  * 2. “事件控制”，“重映射”，“调试IO配置”，“外部中断”，时需要开启AFIO
  *
  ******************************************************************************
  */
#include <conf_gpio.h>

void GPIO_Config(void){

	MX_GPIO_output_Init();
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_output_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  
  /*Configure GPIO pin : PB01 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   /*Configure GPIO pin : PB01 */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); 


	__HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin=GPIO_PIN_0;            //PA0
    GPIO_InitStruct.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_InitStruct.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**
 * @author lhx
 * @time Apr 8, 2020
 * @brief : init event of GPIO. 当触发事件时对芯片内的其他模块发出脉冲触发信号，
			其他模块自动开始动作，不需要CPU处理，效率极高
 */
void MX_GPIO_EVENT_Init(void)
{
	  __HAL_RCC_AFIO_CLK_ENABLE();

	  /* GPIO Ports Clock Enable */
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  __HAL_RCC_GPIOC_CLK_ENABLE();

	  /*EVENT IN*/
	  /*Configure GPIO pin : PC2 */
	  GPIO_InitStruct.Pin = GPIO_PIN_2;
	  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  /*EVENT OUT*/
	  /*Configure GPIO pin : PC3 */
	  GPIO_InitStruct.Pin = GPIO_PIN_3;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  /*Configures the port and pin on which the EVENTOUT Cortex signal will be connected */
	  HAL_GPIOEx_ConfigEventout(AFIO_EVENTOUT_PORT_C, AFIO_EVENTOUT_PIN_3);

	  /*Enables the Event Output */
	  HAL_GPIOEx_EnableEventout();
}

/**
 * @author lhx
 * @time Sep 16, 2020
 * @brief : init interruption of GPIO. 触发中断，CPU自动跳转
	示例：配置 PAO 引脚开启EXTI0_IRQn 上升沿中断。
 * @note 需要在 stm32f10x_it.c 中配置中断处理函数
 */
void MX_GPIO_EXIT_Init(void)
{
	MSG_BSTART("EXIT","init");

	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* 配置 AFIO_EXTICRx 寄存器需要开启 AFIO 时钟 */
	  /* 其实，库函数内部会自动开启 */
//	  __HAL_RCC_AFIO_CLK_ENABLE();
	  
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  /*Configure GPIO pin : PA0 */
	  GPIO_InitStruct.Pin = GPIO_PIN_0;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	   //中断线0-PA0
	  HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
	  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	MSG_ASTART("EXIT","init");
}


/**
 * @author lhx
 * @time Apr 8, 2020
 * @brief : init interruption of GPIO. 触发中断，CPU自动跳转
	示例：PA0 引脚上升沿中断,回调函数
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_Delay(100);
	switch(GPIO_Pin)
	{
		case GPIO_PIN_0:
			if( HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1 ) {
				printf("rising\r\n");
			}
			break;
	}
}


void EXTI0_IRQHandler(void){ 
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
void EXTI1_IRQHandler(void){ 
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
void EXTI2_IRQHandler(void){ 
while(1);
}
void EXTI3_IRQHandler(void){ 
while(1);
}
void EXTI4_IRQHandler(void){ 
while(1);
}

