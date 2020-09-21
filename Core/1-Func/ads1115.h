//i2c_ads1115.h
#ifndef __IIC_ADS1115_H
#define __IIC_ADS1115_H

#include "conf_iic.h"

#define ADS1115_WRITE_ADDRESS        0x90
#define ADS1115_READ_ADDRESS         0x91


/**
  * @brief  CONFIG REG CONFG
  */

#define CONFIG_REG_H     ADS1115_REG_CONFIG_OS_START|\
                         ADS1115_REG_CONFIG_MUX_Diff_01|\
                         ADS1115_REG_CONFIG_PGA_4|\
                         ADS1115_REG_CONFIG_MODE_CONTIN
#define CONFIG_REG_L    ADS1115_REG_CONFIG_DR_128|\
                        ADS1115_REG_CONFIG_COMP_MODE_TRADITIONAL|\
                        ADS1115_REG_CONFIG_COMP_POL_LOW|\
                        ADS1115_REG_CONFIG_COMP_LAT_NONLATCH|\
                        ADS1115_REG_CONFIG_COMP_QUE_DIS


/***************/
//???????

//config register
/*CRH[15:8](R/W)
   BIT      15      14      13      12      11      10      9       8
   NAME     OS      MUX2    MUX1    MUX0    PGA2    PGA1    PGA0    MODE
CRL[7:0] (R/W)
   BIT      7       6       5       4       3       2       1       0
   NAME    DR0     DR1     DR0   COM_MODE COM_POL COM_LAT COM_QUE1 COM_QUE0


   -----------------------------------------------------------------------------------
 * 15    | OS             |  ???????????
 *       |                | ??:
 *       |                | 0   : ??
 *       |                | 1   : ?????????????
 *       |                | ??:
 *       |                | 0   : ????
 *       |                | 1   : ?????
 * -----------------------------------------------------------------------------------
 * 14:12 | MUX [2:0]      | ????????
 *       |                | 000 : AINP = AIN0 and AINN = AIN1 (default)
 *       |                | 001 : AINP = AIN0 and AINN = AIN3
 *       |                | 010 : AINP = AIN1 and AINN = AIN3
 *       |                | 011 : AINP = AIN2 and AINN = AIN3
 *       |                | 100 : AINP = AIN0 and AINN = GND
 *       |                | 101 : AINP = AIN1 and AINN = GND
 *       |                | 110 : AINP = AIN2 and AINN = GND
 *       |                | 111 : AINP = AIN3 and AINN = GND
 * -----------------------------------------------------------------------------------
 * 11:9  | PGA [2:0]      | ??????????(FSR  full scale range)
 *       |                | 000 : FSR = ?±6.144 V
 *       |                | 001 : FSR = ?±4.096 V
 *       |                | 010 : FSR = ?±2.048 V (??)
 *       |                | 011 : FSR = ?±1.024 V
 *       |                | 100 : FSR = ?±0.512 V
 *       |                | 101 : FSR = ?±0.256 V
 *       |                | 110 : FSR = ?±0.256 V
 *       |                | 111 : FSR = ?±0.256 V
 * -----------------------------------------------------------------------------------
 * 8     | MODE           | ????
 *       |                | 0   : ????
 *       |                | 1   : ????
 * -----------------------------------------------------------------------------------
 * 7:5   | DR [2:0]       | ????
 *       |                | 000 : 8 SPS
 *       |                | 001 : 16 SPS
 *       |                | 010 : 32 SPS
 *       |                | 011 : 64 SPS
 *       |                | 100 : 128 SPS (??)
 *       |                | 101 : 250 SPS
 *       |                | 110 : 475 SPS
 *       |                | 111 : 860 SPS
 * -----------------------------------------------------------------------------------
 * 4     | COMP_MODE      | ?????
 *       |                | 0   : ????? (default)
 *       |                | 1   : ?????
 * -----------------------------------------------------------------------------------
 * 3     | COMP_POL       | Comparator polarity
 *       |                | 0   : ????? (default)
 *       |                | 1   : ?????
 * -----------------------------------------------------------------------------------
 * 2     | COMP_LAT       | Latching comparator
 *       |                | 0   : ??????. (default)
 *       |                | 1   : ?????.
 * -----------------------------------------------------------------------------------
 * 1:0   | COMP_QUE [1:0] | Comparator queue and disable
 *       |                | 00  : Assert after one conversion
 *       |                | 01  : Assert after two conversions
 *       |                | 10  : Assert after four conversions
 *       |                | 11  : ???????ALERT/RDY?????? (default)
 * -----------------------------------------------------------------------------------
*/



//???????

#define ADS1015_REG_POINTER_CONVERT     (0x00)
#define ADS1015_REG_POINTER_CONFIG      (0x01)
#define ADS1015_REG_POINTER_LOWTHRESH   (0x02)
#define ADS1015_REG_POINTER_HITHRESH    (0x03)
// ??????
#define ADS1115_REG_CONFIG_OS_START                     (0x1U << 7)//???????? ???????
#define ADS1115_REG_CONFIG_OS_NULL                      (0x0U << 7)
//?????????????
#define ADS1115_REG_CONFIG_MUX_Diff_01                  (0x0U << 4)  // ????0???1??
#define ADS1115_REG_CONFIG_MUX_Diff_03                  (0x1U << 4)  // ????0???3??
#define ADS1115_REG_CONFIG_MUX_Diff_13                  (0x2U << 4)  // ????1???3??
#define ADS1115_REG_CONFIG_MUX_Diff_23                  (0x3U << 4)  // ????2???3??
#define ADS1115_REG_CONFIG_MUX_SINGLE_0                 (0x4U << 4)  //???? 0
#define ADS1115_REG_CONFIG_MUX_SINGLE_1                 (0x5U << 4)  //???? 1
#define ADS1115_REG_CONFIG_MUX_SINGLE_2                 (0x6U << 4)  //???? 2
#define ADS1115_REG_CONFIG_MUX_SINGLE_3                 (0x7U << 4)  //???? 3

//????
#define ADS1115_REG_CONFIG_PGA_6                        (0x0U << 1) // +/- 6.1144
#define ADS1115_REG_CONFIG_PGA_4                        (0x1U << 1) // +/- 4.096
#define ADS1115_REG_CONFIG_PGA_2                        (0x2U << 1) // +/- 2.048
#define ADS1115_REG_CONFIG_PGA_1                        (0x3U << 1) // +/- 1.024
#define ADS1115_REG_CONFIG_PGA_05                       (0x4U << 1) // +/- 0.512
#define ADS1115_REG_CONFIG_PGA_02                       (0x5U << 1) // +/- 0.256

//????

#define ADS1115_REG_CONFIG_MODE_SINGLE              (0x1U << 0)  //  ??
#define ADS1115_REG_CONFIG_MODE_CONTIN              (0x0U << 0)  //????

//????

#define ADS1115_REG_CONFIG_DR_8                     (0x0U << 5)
#define ADS1115_REG_CONFIG_DR_16                    (0x1U << 5)
#define ADS1115_REG_CONFIG_DR_32                    (0x2U << 5)
#define ADS1115_REG_CONFIG_DR_64                    (0x3U << 5)
#define ADS1115_REG_CONFIG_DR_128                   (0x4U << 5)
#define ADS1115_REG_CONFIG_DR_250                   (0x5U << 5)
#define ADS1115_REG_CONFIG_DR_475                   (0x6U << 5)
#define ADS1115_REG_CONFIG_DR_860                   (0x7U << 5)

//?????
#define ADS1115_REG_CONFIG_COMP_MODE_TRADITIONAL    (0x0U << 4)//??
#define ADS1115_REG_CONFIG_COMP_MODE_WINDOW         (0x1U << 4)


#define ADS1115_REG_CONFIG_COMP_POL_LOW         (0x0U << 3)//??
#define ADS1115_REG_CONFIG_COMP_POL_HIG         (0x1U << 3)


#define ADS1115_REG_CONFIG_COMP_LAT_NONLATCH        (0x0U << 2)
#define ADS1115_REG_CONFIG_COMP_LAT_LATCH           (0x1U << 2)


#define ADS1115_REG_CONFIG_COMP_QUE_ONE         (0x0U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_TWO         (0x1U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_THR         (0x2U << 0)
#define ADS1115_REG_CONFIG_COMP_QUE_DIS         (0x3U << 0)


void ads1115_init(I2C_HandleTypeDef *ads1115_I2cHandle, uint8_t configH, uint8_t configL);
int16_t ads1115_read_data(I2C_HandleTypeDef *ads1115_I2cHandle);
double ads1115_get_voltage_val(I2C_HandleTypeDef *ads1115_I2cHandle, uint8_t configH);


TEST ADS_Sampling(void);
	
#endif
