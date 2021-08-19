/*
 * calendar.c
 *
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

/*
 * author lhx
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

/*
 * author lhx
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

/*
 * author lhx
 * Apr 11, 2020
 *
 * @brief : 设置闹铃，并库自动启动中断，因此这里初始化RTC时不需要提前使能中断，只需要开启NVIC中断即可
 * Window > Preferences > C/C++ > Editor > Templates.
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

TEST testalarm(void)
{
	SysTime sys = {0};
	sys.Hour = 1;
	sys.Minute = 30;
	sys.Second = 20;
	setSysTime(sys, 0);

	sys.Minute = 31;
	setAlarm(sys);
}



void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
	getSysTime();
	print_sysTime();
}

void HAL_RTCEx_RTCEventErrorCallback(RTC_HandleTypeDef *hrtc)
{
	MSG("error\r\n");
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	MSG("alarming！！！！！\r\n");
}

TEST systemTimer_test(TEST)
{
	
	systemTimer_init();
}
