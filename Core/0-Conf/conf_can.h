/**
  ******************************************************************************
  * @文件名     ： bsp_can.h
  * @作者       ： strongerHuang
  * @版本       ： V1.0.0
  * @日期       ： 2018年11月14日
  * @摘要       ： CAN底层头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _conf_CAN_H
#define _conf_CAN_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "device_bsp.h"
extern CAN_HandleTypeDef hcan1;


#define SJW SyncJumpWidth
#define BS1 TimeSeg1
#define BS2 TimeSeg2
#define TTCM TimeTriggeredMode
#define ABOM AutoBusOff
#define AWUM AutoWakeUp
#define NART AutoRetransmission
#define RFLM ReceiveFifoLocked
#define TXFP TransmitFifoPriority



/* 宏定义 --------------------------------------------------------------------*/
#if 1 /* 主站板卡定义 */
#define CANx                      CAN1
#define pHCANx                     & hcan1
#define CAN_RX_PIN                GPIO_PIN_0
#define CAN_TX_PIN                GPIO_PIN_1
#define CAN_GPIO_PORT             GPIOB                    //同一个PORT
#define CAN_RX_Priority           10                       //中断函数接口(见stm32f10x_it.c)
#define CAN_RX_IRQHandler         CAN1_RX1_IRQHandler

#else /* 从站板卡定义 */

#define CANx                      CAN1
#define CAN_CLK                   RCC_APB1Periph_CAN1
#define CAN_GPIO_CLK              RCC_APB2Periph_GPIOA
#define CAN_RX_PIN                GPIO_Pin_11
#define CAN_TX_PIN                GPIO_Pin_12
#define CAN_GPIO_PORT             GPIOA                    //同一个PORT
#define CAN_RX_IRQn               USB_LP_CAN1_RX0_IRQn
#define CAN_RX_Priority           10                       //中断函数接口(见stm32f10x_it.c)
#define CAN_RX_IRQHandler         USB_LP_CAN1_RX0_IRQHandler
#endif


/**
  * @brief CAN send package structure

	typedef struct
	{
		uint32_t StdId;    Specifies the standard identifier. between 0 and 0x7FF
		uint32_t ExtId;    Specifies the extended identifier. between 0 and 0x1FFFFFFF.
		uint32_t IDE;      Specifies the type of identifier.  #define CAN_ID_STD, #define CAN_ID_EX
		uint32_t RTR;      Specifies the type of frame for the message. #define CAN_RTR_DATA, #define CAN_RTR_REMOT
		uint32_t DLC;      Specifies the length of the frame. between 0 and 8.
		FunctionalState TransmitGlobalTime; Specifies whether the timestamp counter value captured on start
														of frame transmission, is sent in DATA6 and DATA7 replacing Data[6] and Data[7].
														@note: Time Triggered Communication Mode must be enabled.
														@note: DLC must be programmed as 8 bytes, in order these 2 bytes are sent.
														This parameter can be set to ENABLE or DISABLE. 
	} CAN_TxHeaderTypeDef
*/
typedef struct {
	CAN_TxHeaderTypeDef head;
	uint8_t Data[8];						// can message
} CanTxMsg;


/**
  * @brief CAN receive package structure
  */
typedef struct {
	CAN_RxHeaderTypeDef head;
	uint8_t Data[8];
} CanRxMsg;


#define MAIL uint32_t
#define Error HAL_StatusTypeDef

/* 函数申明 ------------------------------------------------------------------*/
void MX_CAN1_Init(uint32_t mode);
void CanFilter_Init(CAN_HandleTypeDef* hcan, uint32_t FILTER_FIFO);
void MX_CAN1_Test_Init(uint32_t mode);

#endif /* _BSP_CAN_H */
