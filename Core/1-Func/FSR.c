#include "FSR.h"


/**
  *****************************************************************************************************************
  * @file    FSR.c
  * @author  xlh 
  * @data	 20201203
  * @brief   FSR reader module. 
  *			 https://detail.tmall.com/item.htm?spm=a230r.1.14.16.151511c5o5ST7Z&id=624056443324&ns=1&abbucket=1
  *           + Initialization and de-initialization functions
  *           + Analog read functions
  *           + Digital read functions
  */


//#define FSR_TEST

#define FORCENUMMAX 1

uint8_t FSRforce[FORCENUMMAX];						// force raw data(adc output) store 




/**
  * @brief  Init Force collector beforce measure force data. 
  *         1. initalize adc1 device
  */

void FSR_Init (void)
{
	MX_ADC1_Init();
	HAL_ADC_Start(&hadc1);                               //ADC
	
}


/**
  * @brief  Get force value through ADC peripheral. It is not calibrated and handled.
  * @retval ADC value 
  */

uint16_t GetFSRForce (void)
{	
    HAL_ADC_PollForConversion(&hadc1, 20);
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);
}


float FSROffset=0;		// Offset value at zero external force 
int fsr_index=0;		// series number of fsr data
/**
  * @brief  Calcuate the offset at n = OffsetWindowsPosition
  * @retval Offset
  */
#define OffsetWindowsPosition 1000
float GetFSROffset(void)
{
	
	if(fsr_index == OffsetWindowsPosition){
		printf("------------------ offset get --------------------\r\n");
	}

	return FSROffset;
}




#ifdef FSR_TEST

uint16_t FSR_force = 0;

TEST FSRCollectExperiment(void)
{
	FSR_Init();
	while(1){
		FSR_force = GetFSRForce();
		printf("%d\r\n", FSR_force);
		HAL_Delay(20);
	}
}


#include "win.h"
TEST FSR_Aver(void)
{
	WIN winbuffer;
	ElementType data[2] = {0};
	int size = 2;
	WinBuffer(&winbuffer, data, size);
//	float avr;
//	float weights[2] = {0.01,0.99};
	float weights[2] = {0.001,0.999};
	
	FSR_Init();
	while(1){
		FSR_force = GetFSRForce();
		addToBuff(&winbuffer ,FSR_force);
		ChangeLastestValue(&winbuffer, AvergeWin(&winbuffer, weights, 2));
		printf("%f\r\n", getLastestValue(winbuffer));
//		printf("%d\r\n", FSR_force);
		HAL_Delay(20);
	}
	
}

#endif
