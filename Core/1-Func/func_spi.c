#include "func_spi.h"



//SPI速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为42Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&hspi1);            //关闭SPI
    hspi1.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    hspi1.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&hspi1);             //使能SPI
    
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}

#define SPI_TEST

#ifdef SPI_TEST

uint8_t startCmd[2] = {0xFF,0xFF};
uint8_t cmd[] = "你好hello123";
uint8_t rcvcmd[20];
TEST spi1_master_test(TEST)
{
	
	MX_SPIx_Init(SPI1, SPI_MODE_MASTER);	//初始化SPI
	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_256); //设置为42M时钟,高速模式
	
	if(HAL_SPI_Transmit(&hspi1, startCmd, 1, 100) != HAL_ERROR){
		HAL_Delay(2000);
//		printf("send");
		HAL_SPI_Transmit(&hspi1, cmd, sizeof(cmd), 1000);
	}
	while(1){}
}


uint8_t start[4];
TEST spi1_slave_test(TEST)
{
	
	MX_SPIx_Init(SPI1, SPI_MODE_SLAVE);		   			    //初始化SPI
	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_256); //设置为42M时钟,高速模式
	
//	HAL_SPI_Transmit(&hspi1, cmd, sizeof(cmd), 1000);
	 
	while(1){
		if(HAL_SPI_Receive_IT(&hspi1, start, 1)){

			if( start[0] == 0xFF){
				printf("get - ");
				HAL_SPI_Receive(&hspi1, rcvcmd, sizeof(cmd), 3000);
				printf("\t%s", rcvcmd);
			}
		}
	}
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI1){
		printf("\t%x\r\n ", start[0]);
	}
}
#endif
