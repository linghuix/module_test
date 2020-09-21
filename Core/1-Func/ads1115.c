
#include "ads1115.h"

//https://www.jianshu.com/p/e0b448995316

#define ADS_TEST

/**
  * @brief  Write to Config register to configure specific function.
  * @param  hdac pointer to a IIC_HandleTypeDef structure that contains
  *         the configuration information for the specified IIC.
  * @param  high 8bit of Config register
  * @param  low  8bit of Config register
  * @retval 
  */
void ads1115_init(I2C_HandleTypeDef * ads1115_I2cHandle, uint8_t configH, uint8_t configL)
{
	MSG_BSTART("ADS1115", "init");
    uint8_t reg_data[3]={ADS1015_REG_POINTER_CONFIG, configH, configL};
	HAL_I2C_Master_Transmit(ads1115_I2cHandle, 00, reg_data, 1, 1000);
    while(HAL_I2C_Master_Transmit(ads1115_I2cHandle, ADS1115_WRITE_ADDRESS, reg_data, 3, 1000) != HAL_OK)
    {
        if(HAL_I2C_GetError(ads1115_I2cHandle) != HAL_I2C_ERROR_AF)
        {
            printf("ads1115 Config Register error!!!\r\n");
        }
    }
	MSG_ASTART("ADS1115", "init");
}


/**
  * @brief  Read data from Conversion Register.
  * 		 1. Write Address Pointer Register to select Conversion Register.
  *          2. Get Conversion Register data.through Read address.
  * @param  hdac pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval 16bit sample data 
  */

int16_t ads1115_read_data(I2C_HandleTypeDef * ads1115_I2cHandle)
{
    int16_t data;
    uint8_t rx_data[2]={0};

    while(HAL_I2C_Master_Transmit(ads1115_I2cHandle, ADS1115_WRITE_ADDRESS, ADS1015_REG_POINTER_CONVERT, 1, 1000) != HAL_OK)
    {
        if(HAL_I2C_GetError(ads1115_I2cHandle) != HAL_I2C_ERROR_AF)
        {
            MSG("ads1115 convert Register error!!!\r\n");
//			Error_Handler()
        }
    }

    while(HAL_I2C_Master_Receive(ads1115_I2cHandle, ADS1115_READ_ADDRESS, rx_data, 2, 1000) != HAL_OK)
    {
        if(HAL_I2C_GetError(ads1115_I2cHandle) != HAL_I2C_ERROR_AF)
        {
            MSG("ads1115 read data error!!!\r\n");
        }
    }

    data=rx_data[0]*256+rx_data[1];
    return data;
}


/**
  * @brief  Get latest converted 16-bit sample data
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Alignment Specifies the data alignment.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data Data to be loaded in the selected data holding register.
  * @retval HAL status
  */

double ads1115_get_voltage_val(I2C_HandleTypeDef * ads1115_I2cHandle, uint8_t configH)
{
    double val;
    uint16_t ad_val;

    HAL_Delay(10);
    ad_val=ads1115_read_data(ads1115_I2cHandle);
	
    if((ad_val==0x7FFF)|(ad_val==0X8000))	//overflow
    {
        ad_val=0;
        MSG("overflow PGA\r\n");
    }

    switch((0x0E&configH)>>1)
    {
        case(ADS1115_REG_CONFIG_PGA_6):
            val=(double)ad_val*187.5/1000000.0;//
        break;
        case(ADS1115_REG_CONFIG_PGA_4):
            val=(double)ad_val*125/1000000.0;
        break;
        case(ADS1115_REG_CONFIG_PGA_2):
            val=(double)ad_val*62.5/1000000.0;
        break;
        case(ADS1115_REG_CONFIG_PGA_1):
            val=(double)ad_val*31.25/1000000.0;
        break;
        case(ADS1115_REG_CONFIG_PGA_05):
            val=(double)ad_val*15.625/1000000.0;
        break;
        case(ADS1115_REG_CONFIG_PGA_02):
            val=(double)ad_val*7.8125/1000000.0;
        break;
    }
    return val;

}

#ifdef ADS_TEST

/**
  * ready for test
 */
TEST ADS_Sampling(void)
{
	MX_I2C1_Init();
	
	HAL_Delay(1000);
	ads1115_init(&hi2c1, CONFIG_REG_H, CONFIG_REG_L);
	
	while(1)
	{
		double val=ads1115_get_voltage_val(&hi2c1,CONFIG_REG_H);
		MSG("val is %f\r\n",val);
		HAL_Delay(100);
	}
}

#endif

