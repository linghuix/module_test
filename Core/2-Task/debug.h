#ifndef __debug_H
#define __debug_H

#include "conf_usart.h"

/**
  * @brief  	debug configuration info
	* @Author   lhx
  */ 

#define PORT huart1				// 选择输出的串口设备

#define BUFF_Printf				// 使用缓冲区进行串口 printf 发送
//#define NO_BUFF_Printf
#define BufferSize 1000		// 使用缓冲区大小，数据流大时可以选择大一些

// software environment set
#define KEIL				  		//使用的环境是keil5


/**
  * @brief  	different Print "function"  提示信息
	* @Author   lhx
  */ 
#define DEBUG_WAR_CONSOLE_ON
#define DEBUG_ERR_CONSOLE_ON

/* Definition of error and warning macros */
/* -------------------------------------- */
#	define MSG(...) 	printf (__VA_ARGS__)

/* Definition of MSG_ERR */
/* --------------------- */
#ifdef DEBUG_ERR_CONSOLE_ON
#    define MSG_ERR(num, string, value) MSG("\r\n%s,%d : 0X%x %s 0X%x\r\n",__FILE__, __LINE__,num, string, value);
#else
#    define MSG_ERR(num, string, value)
#endif

/* Definition of MSG_WAR */
/* --------------------- */
#ifdef DEBUG_WAR_CONSOLE_ON
#    define MSG_WAR(num, string, value) MSG("\r\n%s,%d : 0X%x %s 0X%x \r\n",__FILE__, __LINE__,num, string, value);
#else
#    define MSG_WAR(num, string, value)
#endif


/**
  *   Before and after start. 初始化成功信息
  */
# define MSG_BSTART(device, action) 	MSG( device );MSG(" waiting for ");MSG(action);MSG(" ...\r\n");
# define MSG_ASTART(device, action) 	MSG( device );MSG(" already ");MSG(action);MSG("\r\n");


/* ------------自定义提示信息-------------------------- */

#define Task_MSG(...) 			MSG(__VA_ARGS__)
#define MMSG(...) 					MSG(__VA_ARGS__)
#define CAN_SEND_MSG(...)		//MSG(__VA_ARGS__)
#define CAN_RCV_MSG(...)		//MSG(__VA_ARGS__)
#define TPDO_MSG(...)				//MSG(__VA_ARGS__)
#define SYNC_MSG(...)				//MSG(__VA_ARGS__)
#define ROW_MSG(...)				MSG(__VA_ARGS__)	
#define ERROR(s,...)				MSG("#ERROR %d# ",s);MSG(__VA_ARGS__);MSG("\t--%s,%d\r\n",__FILE__, __LINE__)
#define TEST_MSG(...)				MSG(__VA_ARGS__)
#define MSG_deviceTest(...) 	MSG(__VA_ARGS__)

#define TIME(...) 					MSG(__VA_ARGS__);MSG(" t-%dms\r\n",HAL_GetTick());

#define Error_Handler() ERROR(3,"init error!"); //while(1);


/**
  * @brief  	test the function 
	* @Author   lhx
  */

#define TEST void
#define TEST_ON			// 启动测试


/**
 * @author lhx
 * @Date  May 13, 2020
 *
 * @brief : in - point to null
 * 			out - point to char
 *
 */
struct Buffer{
	char data[BufferSize];
	uint16_t in;
	uint16_t out;
	uint8_t overflow;
};


/* Interface */
char getDebugBuffer(void);
uint8_t addDebugBuffer(char c);
void debug_init(void);


void test_printf(void);
void test_SpeedOfBuffer_printf(void);

/* usart1 IDLE Interrupt service function*/


#endif
