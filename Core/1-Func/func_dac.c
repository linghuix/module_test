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
  *            @arg ChannelOne: ChannelOne>>1 = DAC_CHANNEL_1 selected
  *            @arg ChannelTwo: ChannelTwo>>1 = DAC_CHANNEL_2 selected
  * 		   @arg ChannelOne | ChannelTwo: DAC Channel2 and Channel1 selected both.
  * @param  Alignment Specifies the data alignment. This has nothing to do with configuration.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data Data to be loaded in the selected data holding register. 
  *				when Channel == (ChannelOne | ChannelTwo). 
  * 				the high 16bits will put into Channel_1. the low 16bits will put into Channel_2.
  */

void MX_DAC_SetValue(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Alignment, uint32_t Data)
{
	if(Channel == (ChannelOne | ChannelTwo))
	{
		HAL_DACEx_DualSetValue(hdac, Alignment, Data >> 16, Data & 0xFFH);
	}
	else if(Channel == ChannelOne)
	{
		HAL_DAC_SetValue(hdac, DAC_CHANNEL_1, Alignment, Data);
	}
	else if(Channel == ChannelTwo)
	{
		HAL_DAC_SetValue(hdac, DAC_CHANNEL_2, Alignment, Data);
	}
}


/**
  * @brief  Initalization and enables DAC and ready for the conversion of channel.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg ChannelOne: ChannelOne>>1 = DAC_CHANNEL_1 selected
  *            @arg ChannelTwo: ChannelTwo>>1 = DAC_CHANNEL_2 selected
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
	MSG_BSTART("DAC","start");
	Channel = (Channel>>1);
	
	MX_DAC_Init(Channel, Channel, Trigger_Mode);
	
	MX_DAC_SetValue(hdac, Channel, DAC_ALIGN_12B_R, 0);
	
	while(HAL_DAC_Start(hdac, Channel) != HAL_OK){
		Error_Handler()
	}
	MSG_ASTART("DAC","start");
}

/**
  * @brief  Enable or disable the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg ChannelOne: ChannelOne>>1 = DAC_CHANNEL_1 selected
  *            @arg ChannelTwo: ChannelTwo>>1 = DAC_CHANNEL_2 selected
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
	HAL_DACEx_TriangleWaveGenerate(hdac, Channel>>1, Amplitude);
}


/**
  * @brief  Enable or disable the selected DAC channel wave generation.
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Amplitude Unmask DAC channel LFSR for noise wave generation.
  *          This parameter can be one of the following values:
  *            @arg DAC_LFSRUNMASK_BIT0: Unmask DAC channel LFSR bit0 for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS1_0: Unmask DAC channel LFSR bit[1:0] for noise wave generation. Max noise is (2^2-1)/4095*3.3V = 
  *            @arg DAC_LFSRUNMASK_BITS2_0: Unmask DAC channel LFSR bit[2:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS3_0: Unmask DAC channel LFSR bit[3:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS4_0: Unmask DAC channel LFSR bit[4:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS5_0: Unmask DAC channel LFSR bit[5:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS6_0: Unmask DAC channel LFSR bit[6:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS7_0: Unmask DAC channel LFSR bit[7:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS8_0: Unmask DAC channel LFSR bit[8:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS9_0: Unmask DAC channel LFSR bit[9:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS10_0: Unmask DAC channel LFSR bit[10:0] for noise wave generation
  *            @arg DAC_LFSRUNMASK_BITS11_0: Unmask DAC channel LFSR bit[11:0] for noise wave generation. Max noise is (2^12-1)/4095*3.3V = 3.3V
  * @retval HAL status
  */
  
void Mx_NoiseGenerate(DAC_HandleTypeDef *hdac, uint32_t Channel, uint32_t Amplitude)
{
	HAL_DACEx_NoiseWaveGenerate(hdac, Channel>>1, Amplitude);
}



#ifdef DAC_TEST
#include "BSP.h"
/**
  * @brief  Single DAC with 8bit output test.
  */

TEST Single_DAC_Output_8(void)
{
	/* inital the DAC and start it */
	MX_DAC_Start(&hdac, ChannelOne, DAC_TRIGGER_NONE);
	
	uint8_t dataOutput;
	uint32_t dataOutput32, n = 0;
	while(1){
		dataOutput = 1000*sin(0.01*n)+1200;
		dataOutput32 = 1000*sin(0.01*n)+1200;
		MX_DAC_SetValue(&hdac, ChannelOne, DAC_ALIGN_8B_R, dataOutput);
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
	MX_DAC_Start(&hdac, ChannelOne, DAC_TRIGGER_NONE);
	
	uint32_t dataOutput32, n = 0;
	while(1){
		dataOutput32 = 1000*sin(0.01*n)+1200;
		MX_DAC_SetValue(&hdac, ChannelOne, DAC_ALIGN_8B_R, dataOutput32);
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
	MX_DAC_Start(&hdac, ChannelOne, DAC_TRIGGER_NONE);
	
	uint32_t dataOutput32, n = 0;
	while(1){
		dataOutput32 = 1000*sin(0.01*n)+1200;
		MX_DAC_SetValue(&hdac, ChannelOne, DAC_ALIGN_12B_R, dataOutput32);
		n++;
		int j = 1000;
		while(j--);
		MSG("Output\t%.2f\tmV\t%d \r\n", (float)dataOutput32/4096 * 3300.0, dataOutput32);
	}
}


/**
  * @brief  Triangle output
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  */

TEST Single_DAC_Tri_Output(void)
{
	MX_DAC_Start(&hdac, ChannelOne, DAC_TRIGGER_T4_TRGO);
	MX_DAC_SetValue(&hdac, ChannelOne, DAC_ALIGN_12B_R, 500);
	Mx_TriangleWaveGenerate(&hdac, ChannelOne, DAC_TRIANGLEAMPLITUDE_511);
	
	MX_TIM_CounterInterrupt(TIM4, 100000, 100);		// frequency = 1/(512*2/1000)
	MX_TIM_MasterSet(TIM4, TIM_TRGO_UPDATE);
	HAL_TIM_Base_Start(&htim4);
}

TEST Single_DAC2_Tri_Output(void)
{
	MX_DAC_Start(&hdac, ChannelTwo, DAC_TRIGGER_T4_TRGO);
	Mx_TriangleWaveGenerate(&hdac, ChannelTwo, DAC_TRIANGLEAMPLITUDE_511);
	
	MX_TIM_CounterInterrupt(TIM4, 100000, 100);		// frequency = 1/(512*2/512) = 0.5 Hz
	MX_TIM_MasterSet(TIM4, TIM_TRGO_UPDATE);
	HAL_TIM_Base_Start(&htim4);
}


TEST Single_DAC_Noise_Output(void)
{
	MX_DAC_Start(&hdac, ChannelOne, DAC_TRIGGER_T4_TRGO);
	Mx_NoiseGenerate(&hdac, ChannelOne, DAC_LFSRUNMASK_BITS10_0);
	
	MX_TIM_CounterInterrupt(TIM4, 10000, 100);
	MX_TIM_MasterSet(TIM4, TIM_TRGO_UPDATE);
	HAL_TIM_Base_Start(&htim4);
}

/**
  * @brief  Triangle output 
  */

TEST Dual_DAC_Tri_Output(void)
{
	MX_DAC_Start(&hdac, ChannelOne, DAC_TRIGGER_T4_TRGO);
	MX_DAC_Start(&hdac, ChannelTwo, DAC_TRIGGER_T4_TRGO);
	Mx_TriangleWaveGenerate(&hdac, ChannelOne, DAC_TRIANGLEAMPLITUDE_2047);
	Mx_TriangleWaveGenerate(&hdac, ChannelTwo, DAC_TRIANGLEAMPLITUDE_2047);
	MX_DAC_SetValue(&hdac, ChannelOne|ChannelTwo, DAC_ALIGN_12B_R, 2047<<16 | 0x00);
	
	MX_TIM_CounterInterrupt(TIM4, 1000000, 100);
	MX_TIM_MasterSet(TIM4, TIM_TRGO_UPDATE);
	HAL_TIM_Base_Start(&htim4);
}

#endif


