/*
 * systemTimer.h
 *
 *  Created on: Apr 11, 2020
 *      Author: test
 */

#ifndef TASK_SYSTEMTIMER_H_
#define TASK_SYSTEMTIMER_H_

#include "conf_rtc.h"
#include "debug.h"

typedef struct
{
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;
  uint16_t Year;
  uint8_t Month;
  uint8_t Weekday;
  uint8_t Day;
} SysTime;

extern SysTime systime;
extern SysTime alarmtime;

void systemTimer_init(void);
void showTime(void);

TEST testalarm(void);

TEST systemTimer_test(TEST);
#endif /* TASK_SYSTEMTIMER_H_ */
