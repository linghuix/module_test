
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
  //printf(" HardFault_Handler !!!\r\n");

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

}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
//void PendSV_Handler(void)
//{
//	printf(" PendSV_Handler !!!\r\n");
//
//}

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

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/
/************************************************
�������� �� CAN_RX_IRQHandler
��    �� �� CAN�����ж�
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void CAN1_RX0_IRQHandler(void)
{
	MSG(" CR - ");
	HAL_CAN_IRQHandler(&hcan1);
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	MSG(" CT\r\n");
	HAL_CAN_IRQHandler(&hcan1);
}


/************************************************
�������� �� CANOPEN_TIM_IRQ_Handler
��    �� �� CANOpen��ʱ�ж�
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
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

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{
	debug_IRQ();
//	HAL_UART_IRQHandler(&huart1);
	
//	#ifdef HARD_TEST
//	IDLE_UART_IRQHandler(&huart1);
//	#endif
}

#include "func_RS485.h"
//#define F1912_TEST
//#define rs485_TEST
void USART2_IRQHandler(void)
{

	HAL_UART_IRQHandler(&huart2);			// for func_1912
	
	#ifdef F1912_TEST
	IDLE_UART_IRQHandler(&huart2);			// for func_1912
	#endif
	#ifdef rs485_TEST
	IDLE_UART_IRQHandler(&huart2);
	#endif
}


void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart3);//HAL���ṩ���жϴ�����
}


/*
 * author lhx
 * Apr 11, 2020
 *
 * @brief : 日历时钟设置
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void RTC_IRQHandler(void)
{
	HAL_RTCEx_RTCIRQHandler(&hrtc);
}

void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&hrtc);
}


void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim4);
}
/**
  * @}
  */ 

void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hspi1);
}



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
