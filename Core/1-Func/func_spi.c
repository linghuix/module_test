#include "func_spi.h"



//SPI�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1ʱ��һ��Ϊ42Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
    __HAL_SPI_DISABLE(&hspi1);            //�ر�SPI
    hspi1.Instance->CR1&=0XFFC7;          //λ3-5���㣬�������ò�����
    hspi1.Instance->CR1|=SPI_BaudRatePrescaler;//����SPI�ٶ�
    __HAL_SPI_ENABLE(&hspi1);             //ʹ��SPI
    
}

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //�����յ�������		
}

#define SPI_TEST

#ifdef SPI_TEST

uint8_t startCmd[2] = {0xFF,0xFF};
uint8_t cmd[] = "���hello123";
uint8_t rcvcmd[20];
TEST spi1_master_test(TEST)
{
	
	MX_SPIx_Init(SPI1, SPI_MODE_MASTER);	//��ʼ��SPI
	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_256); //����Ϊ42Mʱ��,����ģʽ
	
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
	
	MX_SPIx_Init(SPI1, SPI_MODE_SLAVE);		   			    //��ʼ��SPI
	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_256); //����Ϊ42Mʱ��,����ģʽ
	
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
