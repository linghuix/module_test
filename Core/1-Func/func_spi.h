#ifndef __SPI_H
#define __SPI_H
#include "conf_spi.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t


void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);




TEST spi1_master_test(TEST);

TEST spi1_slave_test(TEST);

#endif
