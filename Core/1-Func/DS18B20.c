/*
 * DS18B20.c
 *
 *  Created on: May 29, 2020
 *      Author: test
 */

#include "DS18B20.h"

#define D_PIN (GPIO_TypeDef *)GPIOC, (uint16_t)GPIO_PIN_15
#define Delay_source TIM3

#define D_read 			HAL_GPIO_ReadPin(D_PIN)
#define Pullup_D 		HAL_GPIO_WritePin(D_PIN, (GPIO_PinState)1);
#define unsetD 			HAL_GPIO_WritePin(D_PIN, (GPIO_PinState)0);

float Temperature=0;

void DS18B20_Init(void)
{
	MX_TIM_CounterInterrupt(TIM3, 1000000, 1);
	HAL_TIM_Base_Start(&htim3);
}




//void delay_us(uint32_t us)
//{
//	uint32_t time = getTimeCount(Delay_source);
//	while(us> (time-getTimeCount(Delay_source)));
//}


uint8_t Reset_DS18B20(void)
{
	uint8_t exist;
	Pullup_D
	delay_us(80);
	unsetD
	delay_us(500);
	Pullup_D
	delay_us(50);
	if(D_read)
		exist = 0;
	else
		exist = 1;
	delay_us(200);
	return exist;
}

uint8_t ReadOneChar(void)
{
	uint8_t dat = 0;
	for(int i=0;i<8;i++){
		Pullup_D
		delay_us(10);
		unsetD
		dat >>= 1;
		Pullup_D
		if(D_read)
			dat |= 0x80;
		delay_us(42);
	}
	return dat;
}

void WriteOneChar(uint8_t dat)
{
	for(uint8_t i=0;i<8;i++){
		unsetD
		HAL_GPIO_WritePin(D_PIN, (GPIO_PinState)(dat&0x01));
		delay_us(50);
		Pullup_D
		dat >>= 1;
	}
	delay_us(42);
}

char temper[5] = {'1','1','.','2','3'};
uint8_t fg;
void GetTemperature(void)
{
	if(Reset_DS18B20()){
		WriteOneChar(0xcc);//跳过读序列号操作
		WriteOneChar(0x44);//启动转换温度
		delay_us(1500);
	}

	if(Reset_DS18B20()){
		WriteOneChar(0xcc);//跳过读序列号操作
		WriteOneChar(0xbe);//启动转换温度
		delay_us(1500);
	}
	uint8_t tempL = ReadOneChar();
	uint8_t tempH = ReadOneChar();
	if(tempH>0x7F){
		tempL = ~tempL;
		tempH = ~tempH +1 ;
		fg = 0;
	}
	uint32_t sdate = tempL/16+tempH*16;
	temper[0] = sdate/10 + 0x30;	//HEX->ACSII
	temper[1] = sdate%10 + 0x30;
	uint16_t xiaoshu1 = (tempL&0x0f)*10/16;
	temper[3] = xiaoshu1;
	uint16_t xiaoshu2 = (tempL&0x0f)*100/16%10;
	temper[4] = xiaoshu2;
}


