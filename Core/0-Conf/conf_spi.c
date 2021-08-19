/*
 * conf_spi.c
 *
 *  Created on: May 19, 2020
 *      Author: test
 */

#include "conf_spi.h"
void SPIx_NVIC(SPI_HandleTypeDef* hspi);

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

//SPI_MODE_SLAVE
//SPI_MODE_MASTER
void MX_SPIx_Init(SPI_TypeDef * SPI, int masterOrSlave)
{
	if(SPI == SPI1){
		
		MSG_BSTART("spi1","conf");
		  hspi1.Instance = SPI;	//SPI1
		  hspi1.Init.Mode = masterOrSlave; 				//设置SPI工作模式，设置为主模式
		  hspi1.Init.Direction = SPI_DIRECTION_2LINES;  //设置SPI单向或者双向的数据模式:SPI设置为双线模式
		  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;      //设置SPI的数据大小:SPI发送接收8位帧结构
		  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;   //串行同步时钟的空闲状态为高电平
		  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;        //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
		  hspi1.Init.NSS = SPI_NSS_SOFT;                //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制。 SPI_NSS_HARD_OUTPUT/SPI_NSS_SOFT
		  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
		  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
		  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;       //关闭TI模式
		  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_ENABLE;//关闭硬件CRC校验
		  hspi1.Init.CRCPolynomial = 7;                  //CRC值计算的多项式
		  if (HAL_SPI_Init(&hspi1) != HAL_OK){			//初始化
			Error_Handler()
		  }
		MSG_ASTART("spi1","conf");
		  
		__HAL_SPI_ENABLE(&hspi1);                    //使能SPI1
		
		MSG_ASTART("spi1","start");
	}
}



void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  SPIx_NVIC(hspi);
  if(hspi->Instance==SPI1){
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}


void SPIx_NVIC(SPI_HandleTypeDef* hspi)
{
	if(hspi->Instance==SPI1){
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
	}
}
