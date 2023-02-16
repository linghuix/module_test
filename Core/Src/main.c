/**
 * @Document 主函数
 * @Date 2022/08/29
 * @AuthorGroup lhx
 * @Project test sample project. 本项目为STM32f103 arm系列建立了基本代码框架和测试代码。
        涉及多种外设的使用配置，内设的配置和使用场景
 * @Hardware
 * @Software
 
 */

#include "main.h"


int main(void)
{
  BSP_Init();  // 单片机核心硬件层面的初始化
  debug_init();

#ifdef TEST_ON /*---hardware test---*/
	
//	test_RMDmotor_drive();
	test_RMDmotor_sensorlessImpedanceControl_2();
	
//	test_USB_CDC_speed();
// test_motor_torque_control();

// test_systemTimer();
// test_alarm();

// test_CommandReceiver();

// test_GetForce();
// test_GetCurrentForce();
// printf("beginning\r\n");
// FSR_Aver();
// test_acc_collect();
// test_win_buff();
// test_GetCurrentForce();
// test_searchID();
// ADS_Sampling();
// getSingleChannelData();
// test_Command();
// Experiment_I();
// Dual_DAC_Tri_Output();
// getData();
// ADS_Sampling();

// ForceCollectExperiment();
// systemTimer_test();
// debug_init();

// test_ultrasonic();
// LED_init();
//

// test_beep();
// Test_Servos();

// test_printf();
// test_SpeedOfBuffer_printf();
// test_OverflowBuffer_printf();

// CAN_Send_test();
// CAN_Rcv_test();

// current_control();

// FSRCollectExperiment();

// W25QXX_test();

// spi1_slave_test();
//
// spi1_master_test();
// test_GetCurrentForce();
// test_GetCurrentForce();
// test_RS485();

// test_beep();

// MX_GPIO_EXIT_Init();

// test_USART3_communication();


// pos();
#endif


  while (1) {
    printf("running...\r\n");			// prompt that code is running
    HAL_Delay(1000);
  }
  // return 0;
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
