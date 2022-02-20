
#ifndef __conf_usart_H
#define __conf_usart_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "device_bsp.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

/* for Initalization */
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_UART4_UART_Init(void);
void MX_UART5_UART_Init(void);

	 
/* for FUNC folder */
__weak void USART1_IDLE_CallBack_Conf_usart_c(void);
__weak void USART2_IDLE_CallBack_Conf_usart_c(void);
__weak void USART3_IDLE_CallBack_Conf_usart_c(void);
__weak void USART4_IDLE_CallBack_Conf_usart_c(void);
__weak void USART5_IDLE_CallBack_Conf_usart_c(void);

/* for FUNC folder */
__weak void USART1_RX_CallBack_Conf_usart_c(void);
__weak void USART2_RX_CallBack_Conf_usart_c(void);
__weak void USART3_RX_CallBack_Conf_usart_c(void);
__weak void USART4_RX_CallBack_Conf_usart_c(void);
__weak void USART5_RX_CallBack_Conf_usart_c(void);

/* for debug.c */
void IDLE_UART1_IRQHandler (UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
