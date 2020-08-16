/*
 * func_can.c
 *
 *  Created on: Mar 14, 2020
 *      Author: test
 */
#include "func_dac.h"


#define DAC_TEST


/**
  * @brief  Set the specified data holding register value for DAC channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Alignment Specifies the data alignment. This has nothing to do with configuration.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data Data to be loaded in the selected data holding register.
  * @retval HAL status
  */
void MX_DAC_SetValue(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data)
{
	HAL_DAC_SetValue(hdac, Channel, Alignment, Data);
}


/**
  * @brief  Initalization and enables DAC and ready for the conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param Trigger_Mode
  * 		@arg DAC_TRIGGER_NONE
  * 		@arg DAC_TRIGGER_T6_TRGO
  * 		@arg DAC_TRIGGER_T7_TRGO
  * 		@arg DAC_TRIGGER_T2_TRGO
  * 		@arg DAC_TRIGGER_T4_TRGO
  * 		@arg DAC_TRIGGER_EXT_IT9
  * 		@arg DAC_TRIGGER_SOFTWARE 
  * @retval HAL status
  */

void MX_DAC_Start(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Trigger_Mode)
{
	MSG_BSTART("DAC");
	
	MX_DAC_Init(Channel, Channel, Trigger_Mode);
	
	MX_DAC_SetValue(hdac, Channel, DAC_ALIGN_12B_R, 0);
	
	while(HAL_DAC_Start(hdac, Channel) != HAL_OK){
		Error_Handler()
	}
	MSG_ASTART("DAC");
}

/**
  * @brief  Enable or disable the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Amplitude Select max triangle amplitude.
  *          This parameter can be one of the following values:
  *            @arg DAC_TRIANGLEAMPLITUDE_1: Select max triangle amplitude of 1
  *            @arg DAC_TRIANGLEAMPLITUDE_3: Select max triangle amplitude of 3
  *            @arg DAC_TRIANGLEAMPLITUDE_7: Select max triangle amplitude of 7
  *            @arg DAC_TRIANGLEAMPLITUDE_15: Select max triangle amplitude of 15
  *            @arg DAC_TRIANGLEAMPLITUDE_31: Select max triangle amplitude of 31
  *            @arg DAC_TRIANGLEAMPLITUDE_63: Select max triangle amplitude of 63
  *            @arg DAC_TRIANGLEAMPLITUDE_127: Select max triangle amplitude of 127
  *            @arg DAC_TRIANGLEAMPLITUDE_255: Select max triangle amplitude of 255
  *            @arg DAC_TRIANGLEAMPLITUDE_511: Select max triangle amplitude of 511
  *            @arg DAC_TRIANGLEAMPLITUDE_1023: Select max triangle amplitude of 1023
  *            @arg DAC_TRIANGLEAMPLITUDE_2047: Select max triangle amplitude of 2047
  *            @arg DAC_TRIANGLEAMPLITUDE_4095: Select max triangle amplitude of 4095
  * @retval HAL status
  */
  
void Mx_TriangleWaveGenerate(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Amplitude)
{
	HAL_DACEx_TriangleWaveGenerate(hdac, Channel, Amplitude);
}



#ifdef DAC_TEST

/**
  * @brief  Single DAC with 8bit output test.
  */

TEST Single_DAC_Output_8(void)
{
	/* inital the DAC and start it */
	MX_DAC_Start(&hdac, DAC_CHANNEL_1, DAC_TRIGGER_NONE);
	
	uint8_t dataOutput;
	uint32_t dataOutput32, n = 0;
	while(1){
		dataOutput = 1000*sin(0.01*n)+1200;
		dataOutput32 = 1000*sin(0.01*n)+1200;
		MX_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, dataOutput);
		n++;
		int j = 1000;
		while(j--);
		MSG("Output\t%.2f\tmV\t%d\t%d \r\n", (float)dataOutput/256.0 * 3300.0, dataOutput, dataOutput32);
	}
}

/**
  * @brief  See what will happen when DAC output is 8bit while given 16bit data.
  * @result 16bit data will force to 8bit data. It is similar to dataOutput in Single_DAC_Output_8().
  */

TEST Single_DAC_Output_16_to_8(void)
{
	/* inital the DAC and start it */
	MX_DAC_Start(&hdac, DAC_CHANNEL_1, DAC_TRIGGER_NONE);
	
	uint32_t dataOutput32, n = 0;
	while(1){
		dataOutput32 = 1000*sin(0.01*n)+1200;
		MX_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, dataOutput32);
		n++;
		int j = 1000;
		while(j--);
	}
}


/**
  * @brief  Single DAC with 12bit output test.
  */

TEST Single_DAC_Output(void)
{
	/* inital the DAC and start it */
	MX_DAC_Start(&hdac, DAC_CHANNEL_1, DAC_TRIGGER_NONE);
	
	uint32_t dataOutput32, n = 0;
	while(1){
		dataOutput32 = 1000*sin(0.01*n)+1200;
		MX_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dataOutput32);
		n++;
		int j = 1000;
		while(j--);
		MSG("Output\t%.2f\tmV\t%d \r\n", (float)dataOutput32/4096 * 3300.0, dataOutput32);
	}
}


TEST Single_DAC_Tri_Output(void)
{
	MX_DAC_Start(&hdac, DAC_CHANNEL_1, DAC_TRIGGER_T4_TRGO);
	
	Mx_TriangleWaveGenerate(&hdac, DAC_CHANNEL_1, DAC_TRIANGLEAMPLITUDE_255);
}
	
#endif
