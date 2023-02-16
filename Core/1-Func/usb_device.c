/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Device
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"


/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;


/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_DEVICE_Init(void)
{
	// usb clock configuration
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
	
	// usb hardware configuration
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }
}


//#define USBCDC_TEST
#ifdef USBCDC_TEST

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}


void USBWakeUp_IRQHandler(void)
{ 
	while(1);
}
/**
  * @}
  */
// buff 1000字节时，传输速度约 833 kb/s 每次发送10k
// buff 100字节时， 传输速度约 500-1000 kb/s  每次发送1k数据; 2.5k/s 每次发送100k  555.56kb/s 每次发送10k
// buff 10字节时，  传输速度约 140-160 kb/s 每次发送1k数据;  147k-153k/s  每次发送10k

//unsigned char buff[1000] = {"mmabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdd\
//mabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdd\
//	mabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdd\
//	mabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdd\
//	mabcdabcbcdabcdabcdbcdabcdabcdabcddabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdd\
//"};

//unsigned char buff[100] = {"mmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcdd"};

unsigned char buff[10] = {"mmcdablcdd"};

uint8_t result = USBD_BUSY;
uint32_t a,b;
TEST 	test_USB_CDC_speed(TEST)
{
	MX_USB_DEVICE_Init();
	

		while(1){
			a = HAL_GetTick();
			for(int i = 0;i<1000;i++){
//				printf("send package %d\r\n",i);
				while(CDC_Transmit_FS(buff, sizeof(buff)) != USBD_OK);

			}
			b = HAL_GetTick();
			TEST_MSG("%d-%d, usb speed = %.2f k/s \r\n", a,b,10.0*1000.0/(b-a));
			HAL_Delay(1000);
		}
}



//TEST 	test_USB_CDC_speed_2(TEST)
//{
//	
//	MX_USB_DEVICE_Init();
//	
//		TIME("1");
//		while(1){
//			result = CDC_Transmit_FS(buff, sizeof(buff));
//			
//			if(result == USBD_BUSY){TEST_MSG("USB busy\r\n");}
//			else if(result == USBD_FAIL){TEST_MSG("USB fail\r\n");}
//			else{TEST_MSG("USB OK\r\n");}
//			
//			HAL_Delay(2);												//20kb / s
//		}
//		TIME("2");
//}

#endif
