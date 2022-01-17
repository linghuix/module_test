

#include "main.h"


int main(void)
{
	BSP_Init();		//单片机核心硬件层面的初始化
	debug_init();
	
#ifdef TEST_ON
	/*---hardware test---*/
//	hard_test();
//	test_USART1_communication();
//	test_USART1_Rcv();
	

//	test_GetForce();
//	test_GetCurrentForce();
//	printf("beginning\r\n");
//	FSR_Aver();
//	test_acc_collect();
//	test_win_buff();
//	test_GetCurrentForce();
//	test_searchID();
//	ADS_Sampling();
//	getSingleChannelData();
//	test_Command();
//	Experiment_I();
//	Dual_DAC_Tri_Output(); 
//	getData();
//	ADS_Sampling();

//	ForceCollectExperiment();
//	systemTimer_test();
//	debug_init();

//	test_ultrasonic();
//	LED_init();
//	

//	test_beep();
//	Test_Servos();

//	test_printf();
//	test_SpeedOfBuffer_printf();
//	CAN_Send_test();
//	CAN_Rcv_test();

//	current_control();
	
//	FSRCollectExperiment();

//	W25QXX_test();

//    spi1_slave_test();
//	
//    spi1_master_test();
//test_GetCurrentForce();
//	test_GetCurrentForce();
//	test_RS485();
	
//	test_beep();

	MX_GPIO_EXIT_Init();

//test_USART3_communication();


//pos();
#endif


	while(1){ 	
		printf("running...\r\n");
		HAL_Delay(1000);
	}
	//return 0;
 }


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
