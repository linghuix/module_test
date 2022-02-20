/*
 * calendar.c
 * 涉及硬件 RTC，预备寄存器
 *  Created on: Apr 11, 2020
 *      Author: test
 */

#include "systemTimer.h"

SysTime systime;
SysTime alarmtime;


void getDate(void);

char* weekday[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

void systemTimer_init(void)
{
	RTC_conf();
	HAL_RTCEx_SetSecond_IT(&hrtc);
	getDate();
}

/**
 * @author lhx
 * Apr 11, 2020
 *
 * @brief : 获取RTC日期到systime结构体中。 每次Date发生变化都需要调用，以同步
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void getDate(void)
{
	systime.Year = hrtc.DateToUpdate.Year;
	systime.Month = hrtc.DateToUpdate.Month;
	systime.Weekday = hrtc.DateToUpdate.WeekDay;
	systime.Day = hrtc.DateToUpdate.Date;
}

void saveDate(void)
{
	HAL_RTCEx_BKUPWrite(&hrtc, 2, systime.Month);
	HAL_RTCEx_BKUPWrite(&hrtc, 3, systime.Day);
	HAL_RTCEx_BKUPWrite(&hrtc, 4, systime.Year);
}


void setSysTime(SysTime sys,uint8_t isyear)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef DateToUpdate = {0};

	if(!isyear){
		sTime.Hours = sys.Hour;
		sTime.Minutes = sys.Minute;
		sTime.Seconds = sys.Second;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK){
			Error_Handler()
		}
	}
	else{
		DateToUpdate.Month = systime.Month;
		DateToUpdate.Date = sys.Day;
		DateToUpdate.Year = sys.Year;
		if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK){
			Error_Handler()
		}
		getDate();
	}

}

/**
 * @author lhx
 * Apr 11, 2020
 *
 * @brief : 获取时间信息，并每天的0:0:0 更新日期，保存日期到flash中
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void getSysTime(void)
{
	RTC_TimeTypeDef sTime;

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	systime.Hour = sTime.Hours;
	systime.Minute = sTime.Minutes;
	systime.Second = sTime.Seconds;
	if(systime.Hour == 0){
		if(systime.Minute == 0 && systime.Second == 0){
			getDate();
			saveDate();
		}
	}
}

/**
 * @author lhx
 * Apr 11, 2020
 *
 * @brief : 设置闹铃，并库自动启动中断，因此这里初始化RTC时不需要提前使能中断，只需要开启NVIC中断即可
 * @Note 只能设置小时和分钟的闹钟.
 */

void setAlarm(SysTime sys)
{
	RTC_AlarmTypeDef sAlarm = {0};
	sAlarm.AlarmTime.Hours = sys.Hour;
	sAlarm.AlarmTime.Minutes = sys.Minute;
	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
}

void print_sysTime(void)
{
	printf("20%02d-%02d-%02d %02d:%02d:%02d ", systime.Year,systime.Month,systime.Day,systime.Hour,systime.Minute,systime.Second);
	printf("%s\r\n",weekday[systime.Weekday]);
}




//#define SYSTEMTIMER_TEST
#ifdef SYSTEMTIMER_TEST

/**
 * @author lhx
 * @Date 01/18, 2022
 *
 * @brief : 万年历计时器的计时能力测试
 */
TEST test_systemTimer(TEST)
{
	systemTimer_init();
}

/**
 * @author lhx
 * @Date 01/18, 2022
 *
 * @brief : 万年历的闹钟功能测试
 */
TEST test_alarm(TEST)
{
	systemTimer_init();
	
	SysTime sys = {0};		// calendar initalization
	sys.Hour = 1;
	sys.Minute = 30;
	sys.Second = 20;
	setSysTime(sys, 0);		// set calendar time

	sys.Minute = 31;		// set alarm timer
	setAlarm(sys);
}

/* RTC 中断服务函数，读取当前时间打印出来 */
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	getSysTime();
	print_sysTime();
}

void HAL_RTCEx_RTCEventErrorCallback(RTC_HandleTypeDef *hrtc)
{
	MSG("error\r\n");
}

/* 闹钟中断服务函数 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	MSG("alarming!!!!!\r\n");
}

/**
 * @author lhx
 * @date Apr 11, 2020
 *
 * @brief : 日历时钟设置
 * 
 */

void RTC_IRQHandler(void)
{
	printf("RTC it:");
	HAL_RTCEx_RTCIRQHandler(&hrtc);
}

void RTC_Alarm_IRQHandler(void)
{
	printf("ALARM it:");
	HAL_RTC_AlarmIRQHandler(&hrtc);
}
#endif
