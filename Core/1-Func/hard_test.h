 #ifndef _HARD_test_H_
#define _HARD_test_H_

#include "conf_usart.h"


TEST test_USART1_communication(void);
TEST test_USART1_Rcv(void);
TEST IDLE_UART_IRQHandler(UART_HandleTypeDef *huart);


TEST test_USART3_communication(void);

void hard_test(void);
#endif
