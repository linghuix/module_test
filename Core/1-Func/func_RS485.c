
/**
  * @brief  	RS485 ����
  * @hardware 	RS485 , SP3485оƬ  DERE������PA1������
				���Ƶ�Ƭ��
  *	@			
  * @condition 12V
  * @year 2020/08/28
  */


#include "func_RS485.h"
#include "conf_gpio.h"



		// send command
uint8_t rs485_CommandReceive[30] = {0}, rs485_length = 0;	// receive command
uint8_t rs485_receivebyte;							// receive byte from uart


#define TX_ON GPIO_PIN_SET
#define RX_ON GPIO_PIN_RESET

void rs485_init(void)
{
	MX_USART2_UART_Init();
	
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	//PA1���������485ģʽ����  
    GPIO_Initure.Pin=GPIO_PIN_0; 			//PA0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	// �����ж�
	
	HAL_Delay(500);
}

void rs485_TXMode(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, (GPIO_PinState)TX_ON);
}

void rs485_RXMode(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, (GPIO_PinState)RX_ON);
}


/**
  * @brief  Generate Device ID Set rs485_CommandSend and send out.
  * @param  oldID	The previous ID of Sevice
  * @param  ID 		The ID you want to set.
  */
void rs485_send(void)
{
	uint8_t rs485_CommandSend[8] = {0x55,0xAA};	
	rs485_CommandSend[2] = 0x03;
	rs485_CommandSend[3] = 0x01;	
	rs485_CommandSend[4] = 0xAA;
	rs485_CommandSend[5] = 0xBB;	
	rs485_CommandSend[6] = 0xFF;
	rs485_TXMode();
	HAL_UART_Transmit_IT(&huart2, rs485_CommandSend, 7);
}



/**
  * @brief  Read force data. Generate Read force Protocol rs485_CommandSend and send out.
  * @param  ID 		The ID of Device.
  */
void rs485_Start(void)
{
	rs485_RXMode();
	HAL_UART_Receive_IT(&huart2, &rs485_receivebyte, 1);
}

//#define rs485_TEST

#ifdef rs485_TEST
	
/**
  * @brief  ����һ�������жϲο��������ݿ�����һ��һ�ݵģ�����ÿһ�ݸ���Э��ȥ�����ͱȽϼ���
  *                     �ο� https://blog.csdn.net/qq_17351161/article/details/90415407#_27
  *         �������������յ������ݿ�����������������Э��ͷ֡0x55,0xAA+���ݳ���+������+��Ӧ���ܵ�����+У��ͣ�ȥһһ����
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @param  Channel The selected DAC channel.
  *          This parameter can be one of the following values:
  *            @arg DAC_CHANNEL_1: DAC Channel1 selected
  *            @arg DAC_CHANNEL_2: DAC Channel2 selected
  * @param  Alignment Specifies the data alignment.
  *          This parameter can be one of the following values:
  *            @arg DAC_ALIGN_8B_R: 8bit right data alignment selected
  *            @arg DAC_ALIGN_12B_L: 12bit left data alignment selected
  *            @arg DAC_ALIGN_12B_R: 12bit right data alignment selected
  * @param  Data Data to be loaded in the selected data holding register.
  * @retval HAL status
  */

TEST HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance == USART2){
//		TEST_MSG("Usart2 rcv[%d]  ", rs485_length);
//		TEST_MSG("0x%x\t", rs485_receivebyte);
		rs485_CommandReceive[rs485_length] = rs485_receivebyte;
		rs485_length++;
	}
//	TEST_MSG("\r\n");
	HAL_UART_Receive_IT(&huart2, &rs485_receivebyte, 1);
}


/*
 * �����жϣ����ͽ�����תΪ ����ģʽ
 */
TEST HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2){
		TEST_MSG("send ok\r\n");
		rs485_RXMode();
	}
}

/*
 * �����жϣ����մ�����������
 */
TEST USAR_UART_IDLECallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		TEST_MSG("USART2 IDLE IQR Detected: \r\n");
		
		for(int i=0; i<rs485_length; i++){
			TEST_MSG("0x%X\t", rs485_CommandReceive[i]);
		}
		TEST_MSG("\r\n");
		
		rs485_length = 0;
	}
	
}


TEST IDLE_UART_IRQHandler (UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   // IDLE ?
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                     //
            USAR_UART_IDLECallback(huart);                          //
        }
	}
}


TEST test_RS485(void){
	rs485_init();
	rs485_Start();
	rs485_send();
}

#endif
