/*
 * conf_dac.c
 *
 *  Created on: Aug 14, 2020
 *      Author: test
 */
#include "conf_dac.h"

DAC_HandleTypeDef hdac;



/**
  * @brief DAC CHANNEL1 and CHANNEL2 Initialization.
  * @param DAC_CHANNEL_NOTUSE / DAC_CHANNEL_1 / DAC_CHANNEL_2
  * @param Trigger_Mode
  * 		@arg DAC_TRIGGER_NONE
  * 		@arg DAC_TRIGGER_T6_TRGO
  * 		@arg DAC_TRIGGER_T7_TRGO
  * 		@arg DAC_TRIGGER_T2_TRGO
  * 		@arg DAC_TRIGGER_T4_TRGO
  * 		@arg DAC_TRIGGER_EXT_IT9
  * 		@arg DAC_TRIGGER_SOFTWARE 
  * @retval None
  */
void MX_DAC_Init(uint32_t DAC_CHANNEL1, uint32_t DAC_CHANNEL2, uint32_t Trigger_Mode)
{

  DAC_ChannelConfTypeDef sConfig = {0};

  /** DAC Initialization */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler()
  }
  /** DAC channel OUT1 config */
  sConfig.DAC_Trigger = Trigger_Mode;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if(DAC_CHANNEL1 == DAC_CHANNEL_1){
	  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK){
		Error_Handler()
	  }
  }

  /** DAC channel OUT2 config
  */
  if(DAC_CHANNEL2 == DAC_CHANNEL_2){
	  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK){
		Error_Handler()
	  }
  }

}



/**
* @brief DAC MSP Initialization
* This function configures the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hdac->Instance==DAC)
  {
    /* Peripheral clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN DAC_MspInit 1 */

  /* USER CODE END DAC_MspInit 1 */
  }

}


/**
* @brief DAC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
  if(hdac->Instance==DAC)
  {
    /* Peripheral clock disable */
    __HAL_RCC_DAC_CLK_DISABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);

  }
}
