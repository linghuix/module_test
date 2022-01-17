#ifndef __GPIO_H
#define __GPIO_H

#include "device_bsp.h"


/*****************  function declaration ????********************************************/
void GPIO_Config(void); 
void MX_GPIO_output_Init(void);
void MX_GPIO_EXIT_Init(void);


void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
#endif 
