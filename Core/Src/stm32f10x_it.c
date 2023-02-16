
/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "debug.h"
#include  "BSP.h"

#include "func_1912.h"

#include "conf_spi.h"

#include "hard_test.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
	printf(" NMI_Handler !!!\r\n");
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  printf(" HardFault_Handler !!!\r\n");

  while (1)
  {

  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  printf(" MemManage_Handler !!!\r\n");

  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  printf(" BusFault_Handler !!!\r\n");

  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  printf(" UsageFault_Handler !!!\r\n");

  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
	printf(" SVC_Handler !!!\r\n");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
	printf(" DebugMon_Handler !!!\r\n");
	  while (1)
  {
  }
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
	printf(" PendSV_Handler !!!\r\n");
	  while (1)
  {
  }

}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/


void CANOPEN_TIM_IRQ_Handler(void)
{
	MSG(" CANOPEN_TIM_IRQ_Handler !!!\r\n");
	HAL_TIM_IRQHandler(&htim2);

}

void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

void TIM1_UP_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

void TIM1_TRG_COM_IRQHandler(void)
{

	HAL_TIM_IRQHandler(&htim1);

}



void WWDG_IRQHandler(void){ 
while(1);
}
void PVD_IRQHandler(void){ 
while(1);
}
void TAMPER_IRQHandler(void){ 
while(1);
}

void FLASH_IRQHandler(void){ 
while(1);
}
void RCC_IRQHandler(void){ 
while(1);
}

void DMA1_Channel1_IRQHandler(void){ 
while(1);
}
void DMA1_Channel2_IRQHandler(void){ 
while(1);
}
void DMA1_Channel3_IRQHandler(void){ 
while(1);
}
void DMA1_Channel4_IRQHandler(void){ 
while(1);
}
void DMA1_Channel5_IRQHandler(void){ 
while(1);
}
void DMA1_Channel6_IRQHandler(void){ 
while(1);
}
void DMA1_Channel7_IRQHandler(void){ 
while(1);
}
void ADC1_2_IRQHandler(void){ 
while(1);
}


void CAN1_RX1_IRQHandler(void){ 
	printf("CAN rx int\r\n");
	while(1);
}
void CAN1_SCE_IRQHandler(void){ 
	printf("CAN sce int\r\n");
while(1);
}
void EXTI9_5_IRQHandler(void){ 
while(1);
}
void TIM1_BRK_IRQHandler(void){ 
while(1);
}

void TIM2_IRQHandler(void){ 
while(1);
}
void TIM3_IRQHandler(void){ 
while(1);
}

void I2C1_EV_IRQHandler(void){ 
while(1);
}
void I2C1_ER_IRQHandler(void){ 
while(1);
}
void I2C2_EV_IRQHandler(void){ 
while(1);
}
void I2C2_ER_IRQHandler(void){ 
while(1);
}

void SPI2_IRQHandler(void){ 
while(1);
}

void EXTI15_10_IRQHandler(void){ 
while(1);
}


void TIM8_BRK_IRQHandler(void){ 
while(1);
}
void TIM8_UP_IRQHandler(void){ 
while(1);
}
void TIM8_TRG_COM_IRQHandler(void){ 
while(1);
}
void TIM8_CC_IRQHandler(void){ 
while(1);
}
void ADC3_IRQHandler(void){ 
while(1);
}
void FSMC_IRQHandler(void){ 
while(1);
}
void SDIO_IRQHandler(void){ 
while(1);
}
void TIM5_IRQHandler(void){ 
while(1);
}
void SPI3_IRQHandler(void){ 
while(1);
}

void TIM6_IRQHandler(void){ 
while(1);
}
void TIM7_IRQHandler(void){ 
while(1);
}
void DMA2_Channel1_IRQHandler(void){ 
while(1);
}
void DMA2_Channel2_IRQHandler(void){ 
while(1);
}
void DMA2_Channel3_IRQHandler(void){ 
while(1);
}
void DMA2_Channel4_5_IRQHandler(void){ 
while(1);
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
