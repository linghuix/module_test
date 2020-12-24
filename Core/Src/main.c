
#include "main.h"

int main(void)
{
	BSP_Init();		//单片机核心硬件层面的初始化
	debug_init();
	
#ifdef TEST_ON
	
	printf("beginning\r\n");
	FSRCollectExperiment();
//	test_win_buff();
//	test_GetCurrentForce();
//	test_searchID();
//	ADS_Sampling();
//	getSingleChannelData();
//	test_Command();
//	expriment_1();
//	Dual_DAC_Tri_Output(); 
//	getData();
//	ADS_Sampling();

//	ForceCollectExperiment();
//	systemTimer_init();
//	debug_init();

//	test_ultrasonic();
//	LED_init();
//	
//	Servo_Init();
//	Servo_Start()CANOpen_Inital;

//	test_printf();
//	test_SpeedOfBuffer_printf();
//	CAN_Send_test();
//	CAN_Rcv_test();
	
#endif


	while(1){ 	}
	//return 0;
 }


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
