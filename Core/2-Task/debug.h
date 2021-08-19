#ifndef __debug_H
#define __debug_H

#include "conf_usart.h"

#define DEBUG_WAR_CONSOLE_ON
#define DEBUG_ERR_CONSOLE_ON

#define PORT huart1


#define BUFF_Printf				//ʹ�û��������д��� printf ����
#define BufferSize 1000		    //ʹ�û�������С����������ʱ����ѡ���һЩ
//#define NO_BUFF_Printf

#define KEIL					//ʹ�õĻ�����keil5


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
  * @brief  Before and after start. ��ʼ���ɹ���Ϣ
  */
# define MSG_BSTART(device, action) 	MSG( device );MSG(" waiting for ");MSG(action);MSG(" ...\r\n");
# define MSG_ASTART(device, action) 	MSG( device );MSG(" already ");MSG(action);MSG("\r\n");


/* ------------�Զ�����ʾ��Ϣ-------------------------- */

#define Task_MSG(...) 			MSG(__VA_ARGS__)
#define MMSG(...) 				MSG(__VA_ARGS__)
#define CAN_SEND_MSG(...)		//MSG(__VA_ARGS__)
#define CAN_RCV_MSG(...)		//MSG(__VA_ARGS__)
#define TPDO_MSG(...)			//MSG(__VA_ARGS__)
#define SYNC_MSG(...)			//MSG(__VA_ARGS__)
#define ROW_MSG(...)			MSG(__VA_ARGS__)	
#define ERROR(s,...)			MSG("#ERROR %d# ",s);MSG(__VA_ARGS__);MSG("\t--%s,%d\r\n",__FILE__, __LINE__)
#define TEST_MSG(...)			MSG(__VA_ARGS__)
#define MSG_deviceTest(...) 	MSG(__VA_ARGS__)




/*
 * author lhx
 * May 13, 2020
 *
 * @brief : in - point to null
 * 			out - point to char
 * Window > Preferences > C/C++ > Editor > Templates.
 */
struct Buffer{
	char data[BufferSize];
	uint16_t in;
	uint16_t out;
};



char getDebugBuffer(void);
uint8_t addDebugBuffer(char c);
void debug_init(void);
void debug_IRQ(void);


void test_printf(void);
void test_SpeedOfBuffer_printf(void);


#endif
