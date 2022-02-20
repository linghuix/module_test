/**
 * func_BLE.c
 *
 *  Created on: May 25, 2020
 *      Author: test
 *
 *  use USART 1 RX function
 *  @brief: ʵ����λ��ͨ������(����/����)���ڿ��Ƶ�Ƭ��
			����ʵ��Ӳ���ĳ�ʼ���� Command �Ľ���
 */

#include "func_BLE_HC05.h"

#define COMMANDLENGTH 200		//bytes

/**
 * @author lhx
 * @Date 01/17, 2020
 *
 * @brief : BLE Command receiver Initalization
 */
uint8_t BLE_CommandReceive[COMMANDLENGTH], BLE_receivebyte, BLE_length;
void HC05_RcvCmd_Init(void)
{
	MX_USART1_UART_Init();
}

void HC05_RcvCmd_Start(void)
{
	HAL_UART_Receive_IT(&huart1, &BLE_receivebyte, 1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	// �����ж�
}

/**
 * @author lhx
 * @Date 01/17, 2020
 *
 * @brief : USART1_IDLE_CallBack. set flag to notice 
 */
int idleflag = 0;
void USART1_IDLE_CallBack_Conf_usart_c(void)
{
	idleflag = 1;
}

/**
 * @author lhx
 * @Date 01/17, 2020
 *
 * @brief : USART1_RX_CallBack.  BLE_CommandReceive �еĴ������ݽ���
 * @retval None
call inputPara()

 */
void USART1_RX_CallBack_Conf_usart_c(void)
{
	TEST_MSG("get cmad\r\n");
	BLE_CommandReceive[BLE_length] = BLE_receivebyte;
	BLE_length++;
	HAL_UART_Receive_IT(&huart1, &BLE_receivebyte, 1);
}

/**
  ******************************************************************************
  * @section    Test
  * @author  xlh
  * @brief   
  ******************************************************************************
  */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TEST @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


//#define BLE_CMD_TEST
#ifdef BLE_CMD_TEST

/**
  * @brief  ���ڿ����жϻص�����
  * @retval None
  */
TEST test_CommandReceiver(TEST)
{
	HC05_RcvCmd_Init();
	HC05_RcvCmd_Start();
	
	TEST_MSG("please input 11 int split by '/'. the first int must be 170. input string must end with Enter\r\n");
	TEST_MSG("for example: \"170/5/6/9/4/4/5/5/8/9/7\\r\\n\"\r\n");
	
	/* idle �������ݽ��� */
	while(1) {
		if( idleflag == 1) {
			commandPrase();
			idleflag = 0;
		}
	}
}


/**
  * @brief  �洢�����������Ľṹ��,����ܹ��洢50��int����
  */
struct parasturct
{
	int para[50];		// �������������������
	uint8_t paranum;	// �������������������
};
struct parasturct para = {0};


/**
  * @brief  BLE_CommandReceive �еĴ������ݽ���
  * @retval None
call inputPara()
  */
void inputPara(struct parasturct * parasturct, uint8_t * paradata, uint8_t length);
void commandPrase(void)
{
	TEST_MSG("Received command: %s",BLE_CommandReceive );
	TEST_MSG("Received command length: %d\r\n", BLE_length );
	
	inputPara(&para, BLE_CommandReceive, BLE_length);
	
	int flag=1;						// �жϵ�һ�����Ƿ�Ϊ170
	if( para.para[0] == 170 ){		// ��һ�����ֱ�Ϊ170 ������λ 0x10101010
		flag = 0;
	}
	if(flag == 0){
		printf("get number int %d\r\n", para.paranum);
		if ( para.paranum >= 11){
			/* assistive torque curve*/
			int PREDICT_TIME;
			float tao_Ep,fai_Ep,fai_Er,fai_Ef,a[3],b[3];
			tao_Ep = (float)para.para[1]/100.0;		// 5-10 Nm
			fai_Ep = (float)para.para[2]/1000.0; 	// 0.2-0.3
			fai_Er = (float)para.para[3]/1000.0;	// 0.1-0.2
			fai_Ef = (float)para.para[4]/1000.0;	// 0.1-0.2
			
			PREDICT_TIME = para.para[5];
			
			a[0] = (float)para.para[6]/1000.0;
			a[1] = (float)para.para[7]/1000.0;
			a[2] = (float)para.para[8]/1000.0;
			b[0] = (float)para.para[9]/1000.0;
			b[1] = (float)para.para[10]/1000.0;
			b[2] = (float)para.para[11]/1000.0;
			
			TEST_MSG("para change : PREDICT_TIME=%d\r\n", PREDICT_TIME);
			TEST_MSG("para change : [tao_Ep,fai_Ep,fai_Er,fai_Ef]=[%.3f,%.3f,%.3f,%.3f]\r\n", tao_Ep,fai_Ep,fai_Er,fai_Ef);
			TEST_MSG("para change : [a,b]=[[%.3f,%.3f,%.3f],[%.3f,%.3f,%.3f]]\r\n", a[0],a[1],a[2],b[0],b[1],b[2]);
			printf("\r\n#####\r\n");
		}
	}
	BLE_length = 0;
}

/**
	* @brief: ���������ַ���"5/6/7/8\r\n"������������������ֵ��㷨��֧���������֣�����������Ҫ����
	* @note ���������벻ȫ�ģ�������¶�Ӧ�����������������ģ�����¶�Ӧ������������ٵ���ȫ�ģ�����¡�
	* ��/70�� - ������  ��90/80�� ���� ��89/��������
	* @parameter parasturct:	�洢�������ݽṹ��
	* @parameter paradata:		��������������ַ���
	* @parameter length:		��������������ַ����ֽ���
	* @Note input string must end with Enter
is called in commandPrase()
 */
#include<stdlib.h>
TEST inputPara(struct parasturct * parasturct, uint8_t * paradata, uint8_t length)
{
	uint8_t para[50];						// paradata ��ȡ���������ַ���para ��ȡ����
	
	int index = 0;							// index ���
	parasturct->paranum = 0;				// paranum ��������
	
	for(int i=0; i<length; i++){
				
		if(paradata[i] == '/'){
			para[index] = '\0';								// para[]�ַ���Ϊһ������
			parasturct->para[parasturct->paranum++] = atoi( (char *)para );			// �ַ���ת����
			index = 0;
		}
		else if(paradata[i] == '\n'){
			para[index] = '\0';	
			parasturct->para[parasturct->paranum++] = atoi( (char *)para );
			break;
		}
		else{
			para[index++] = paradata[i];
		}
	}
	
//	uint8_t output[2];						//�������
//	for(int i=0;i<parasturct->paranum;i++){

//		HAL_UART_Transmit(&huart1, (uint8_t *)&parasturct->para[i], sizeof(output), 500);
//		output[0] = 0xFF;
//		HAL_UART_Transmit(&huart1, output, 1, 500);
//	}
}

#endif
