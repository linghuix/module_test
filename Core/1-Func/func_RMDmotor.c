/**
 *
 *  Created on: 9 30, 2022
 *  Author: lhx
 *
 *  @hardware RMDϵ�е�� / STM32F103 ������ / 24V /5V
 *  @brief  ʹ��Ƕ��ʽ�豸ͨ��Motech���������Ƴ����JA90���ģ�飬ʵ��Ħ����������
 *  @ref D:\����\5-רҵ֪ʶ\0-Ӳ��\100-Ӳ������\2-������\1-�������ܵ��\RMDϵ��
 */

#include "func_RMDmotor.h"

#define COMMAND_GET_CURRENT 0x40
#define COMMAND_GET_SPEED 0x3F
#define COMMAND_GET_POS 0x05

#define RMD_COMMAND_SIZE 13


/**
 * @date   2022/9/30
 * @author glh
 * @brief  �ú������ڽ�ָ��ĺ��������ֽڼ���crcУ����
 * @param
        array_of_databytes - �����CRCУ�����ָ�
        number_of_databytes-2���ֽڵ�crc��Ҫ�����λ�ã�0��ʾ�ӵ�һ���ֽڲ���crc
        commands - CRC������ָ��. ����ȫ�ֱ��� commands[13];
 * @return
 */
#define POLYNOMIAL 0xA001
#define PRESET_VALUE 0xFFFF
void CRC16func(uint8_t array_of_databytes[], uint8_t number_of_databytes, uint8_t commands[])
{
  uint16_t current_crc_value;
  uint8_t  hi, lo;
  int      i, j;
  current_crc_value = PRESET_VALUE;  //����#define PRESET_VALUE 0xFFFF
  for (i = 0; i < number_of_databytes; i++) {
    current_crc_value = current_crc_value ^ ((unsigned int)array_of_databytes[i]);
    for (j = 0; j < 8; j++) {
      if (current_crc_value & 0x0001) {
        current_crc_value =
            (current_crc_value >> 1) ^ POLYNOMIAL;  //����#define POLYNOMIAL 0xA001 ����������0x8408
                                                    //// x^16 + x^12 + x^5 + 1
      }
      else {
        current_crc_value = (current_crc_value >> 1);
      }
    }
  }
  //�е�У�鷽����Ҫ��Խ��ȡ�����У�����Ҫ�����������
  // current_crc_value = ~current_crc_value;
  //����һ�¸ߵ�λ���е��㷨��Ҫ����һ�£��еĲ���Ҫ
  hi = (current_crc_value >> 8) & 0xff;
  lo = current_crc_value & 0xff;
  // current_crc_value = ((lo<<8) | hi);


  for (int k = 0; k < (number_of_databytes + 2); k++) {
    if (k < number_of_databytes) {
      commands[k] = array_of_databytes[k];
    }
    else if (k == number_of_databytes) {
      commands[k] = lo;
    }
    else if (k == number_of_databytes + 1) {
      commands[k] = hi;
    }
  }
}

/**
 * @author lhx
 * @date 08/29, 2022
 *
 * @brief : motech driver initalization
 */
#define RMD_huart huart3
uint8_t RMDmotor_echo_byte = 0;  // RMDmotor driver echo byte
void    RMDmotor_Init(void)
{
  MX_USART3_UART_Init();

  HAL_UART_Receive_IT(&RMD_huart, &RMDmotor_echo_byte, 1);
  __HAL_UART_ENABLE_IT(&RMD_huart, UART_IT_IDLE);  // �����ж�
}

/**
 * @date   2022/9/30
 * @author lhx
 * @brief  �����ջ�����
 * @param
        commands - command has 13 bytes
        iqcontrol_mA - ��Ҫ���Ƶĵ�����С,��λ mA
 * @return
 * @note   ����᷵�� �¶�[int8_t 1��/LSB]��Iq[int16_t 0.01A/LSB]���ٶ�[int16_t
 1dps/LSB]�ͽǶ���Ϣ[int16_t 1degree/LSB]
 */
void RMDmotor_setCurrent(uint8_t commands[], int16_t iqcontrol_mA)
{
  iqcontrol_mA        = iqcontrol_mA / 10;
  uint8_t commandSize = 11;
  uint8_t current[11] = {0x3E, 0x01, 0x08, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t hi, lo;
  hi         = (iqcontrol_mA >> 8) & 0xff;
  lo         = iqcontrol_mA & 0xff;
  current[7] = lo;
  current[8] = hi;
  CRC16func(current, commandSize, commands);

  HAL_UART_Transmit_IT(&RMD_huart, commands, RMD_COMMAND_SIZE);
}

/**
 * @author lhx
 * @Date 10/2, 2022
 *
 * @brief : RMDmotor read Temperature, Iq, Angular speed, and Angle.
 * @param
        commands - command has 13 bytes
 */
void RMDmotor_read_Temperature_Iq_Speed_Angle(uint8_t commands[])
{
  uint8_t commandSize = 11;
  uint8_t current[11] = {0x3E, 0x01, 0x08, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  CRC16func(current, commandSize, commands);

  HAL_UART_Transmit_IT(&RMD_huart, commands, RMD_COMMAND_SIZE);
}


/**
 * @author lhx
 * @Date 10/2, 2022
 *
 * @brief : RMDmotor_read_acceleration. uint8_t commands[13]
 * @param
        commands - command has 13 bytes
 */
void RMDmotor_read_acceleration(uint8_t commands[])
{
  uint8_t current[11] = {0x3E, 0x01, 0x08, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  CRC16func(current, 11, commands);

  HAL_UART_Transmit_IT(&RMD_huart, commands, RMD_COMMAND_SIZE);
}


/**
 * @date   2022/9/8
 * @author lhx
 * @brief  move the motor to a absolute position.
 * @param
                commands - command has 13 bytes
                maxSpeed - speed limitation of motor (unit in 1dps/LSB). �����˵�������
ת��������ٶ�
                Angle_degree - (degree/LSB)

 * @return
 * @note   ���������˶������Ժ���Ҫ���͡���ʼ�˶���ָ�����ŻῪʼ�˶���
 */
void RMDmotor_moveAbs(uint8_t commands[], uint16_t maxSpeed, int32_t Angle_degree)
{
  uint32_t Angle       = Angle_degree * 100;  // 0.01degree
  uint8_t  current[11] = {0x3E, 0x01, 0x08, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  current[2] = (uint8_t)maxSpeed;
  current[3] = (uint8_t)maxSpeed >> 8;
  current[4] = (uint8_t)Angle;
  current[5] = (uint8_t)Angle >> 8;
  current[6] = (uint8_t)Angle >> 16;
  current[7] = (uint8_t)Angle >> 24;

  CRC16func(current, 11, commands);

  HAL_UART_Transmit_IT(&RMD_huart, commands, RMD_COMMAND_SIZE);
}

/**
 * @date   2022/9/8
 * @author lhx
 * @brief  ����reset motor
 * @param
        commands - command has 13 bytes
        maxSpeed - speed limitation of motor (unit in 1dps/LSB). �����˵�������
ת��������ٶ�
        Angle_degree - (degree/LSB)

 * @return
 * @note   ���������˶������Ժ���Ҫ���͡���ʼ�˶���ָ�����ŻῪʼ�˶���
 */
void RMDmotor_reset(uint8_t commands[])
{
  uint8_t current[11] = {0x3E, 0x01, 0x08, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  CRC16func(current, 11, commands);

  HAL_UART_Transmit_IT(&RMD_huart, commands, RMD_COMMAND_SIZE);
}


/**
 * @date   2022/10/2
 * @author lhx
 * @brief  ����Motor�ı�������ֵΪ��
 * @param
        commands - command has 13 bytes
        maxSpeed - speed limitation of motor (unit in 1dps/LSB). �����˵�������
ת��������ٶ�
        Angle_degree - (degree/LSB)

 * @return
 * @note   ������������Ч
 */
void RMDmotor_setCurrentZero(uint8_t commands[])
{
  uint8_t current[11] = {0x3E, 0x01, 0x08, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  CRC16func(current, 11, commands);

  HAL_UART_Transmit_IT(&RMD_huart, commands, RMD_COMMAND_SIZE);
}

/**
 ******************************************************************************
 * @section    Test
 * @author  xlh
 * @brief
 ******************************************************************************
 */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TEST @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#define MOTECH_TEST
#ifdef MOTECH_TEST

/**
 * @author lhx
 * @Date 08/29, 2022
 *
 * @brief : USART3_IDLE_CallBack. set flag to notice
 */
#define SETCURRENT 0xA1  // command ID of setting current
#define READACCELERATION 0x42  // command ID of setting current
#define MAX_ECHO 20

uint8_t RMDmotor_echo[MAX_ECHO] = {0};  // RMDmotor driver echo information. 13bytes
uint8_t RMDmotor_cmd_length     = 0;    // RMDmotor control command byte number.
int     RMDmotor_idleflag       = 0;    // �����ж�flag

int8_t  RMDmotor_temperature;  // ���϶�
int16_t speed_dps;             // ����ٶ�ֵ dps
float   Iq_A;                  // ���ʵ��q����� A
int16_t angle_degree;          // ���ʵ�ʱ�������ֵ��degree
int32_t acceleration_dpss;     // ���ʵ�ʼ��ٶ� 1 dps/s

void USART3_IDLE_CallBack_Conf_usart_c(TEST)
{
  // TEST_MSG("get cmad\r\n");
  // Bufffprint(RMDmotor_echo, 13);

  // Iq set return
  if (RMDmotor_echo[3] == SETCURRENT && RMDmotor_cmd_length == 13) {
    RMDmotor_temperature = RMDmotor_echo[4];
    Iq_A                 = 0.01 * (int16_t)(RMDmotor_echo[6] << 8 | RMDmotor_echo[5]);
    speed_dps            = RMDmotor_echo[8] << 8 | RMDmotor_echo[7];
    angle_degree         = RMDmotor_echo[10] << 8 | RMDmotor_echo[9];
  }
  // acceleration return
  else if (RMDmotor_echo[3] == READACCELERATION && RMDmotor_cmd_length == 13) {
    acceleration_dpss =
        RMDmotor_echo[10] << 24 | RMDmotor_echo[9] << 16 | RMDmotor_echo[8] << 8 | RMDmotor_echo[7];
  }
  else {
    printf("Nofit\r\n");
  }

  RMDmotor_cmd_length = 0;
}

/**
 * @author lhx
 * @Date 08/29, 2022
 *
 * @brief : USART3_RX_CallBack.  RMDmotor_idleflag �еĴ������ݽ���
 * @retval None
 */
void USART3_RX_CallBack_Conf_usart_c(TEST)
{
  RMDmotor_echo[RMDmotor_cmd_length] = RMDmotor_echo_byte;
  RMDmotor_cmd_length++;
  if (RMDmotor_cmd_length == MAX_ECHO) {
    RMDmotor_cmd_length = 0;
  }
  HAL_UART_Receive_IT(&RMD_huart, &RMDmotor_echo_byte, 1);
}


uint8_t COMMAND[RMD_COMMAND_SIZE];


/**
 * @author lhx
 * @date 09/3, 2022
 *
 * @brief  drive the motor in current mode
 * @retval None
 */
TEST test_RMDmotor_drive(TEST)
{
  RMDmotor_Init();
  HAL_Delay(500);

  RMDmotor_setCurrentZero(COMMAND);
  HAL_Delay(10);
  RMDmotor_reset(COMMAND);
  HAL_Delay(10);

  RMDmotor_setCurrent(COMMAND, 50);
  HAL_Delay(500);

  while (1) {
    RMDmotor_setCurrent(COMMAND, 50);
    printf("speed = %5d,Iq=%.2f,degree = %5d\r\n", speed_dps, Iq_A, angle_degree);
    HAL_Delay(10);
  }
}


/**
 * @date   2022/9/26
 * @author lhx
 * @brief  admittance control ͨ��q����������ⲿ����ͨ��Ԥ���ⲿ��-λ�ù�ϵ��������λ��
 * @param  parameters definition
 * @return
 * @note   usually the special usage need to be noticed
 */
TEST test_RMDmotor_sensorlessImpedanceControl(TEST)
{
  int32_t initalPosition = 0, ImpedancePosition;
  int16_t Iq_change      = 0;     // change of Iq value
  float   k              = 0.05;  // stiffness of impedance control
  RMDmotor_Init();
  HAL_Delay(500);

  RMDmotor_setCurrentZero(COMMAND);
  HAL_Delay(10);
  RMDmotor_reset(COMMAND);
  HAL_Delay(10);

  RMDmotor_moveAbs(COMMAND, 90, initalPosition);
  HAL_Delay(100);


  while (1) {
    ImpedancePosition = initalPosition - Iq_change * k / 10;

    RMDmotor_moveAbs(COMMAND, 90, initalPosition);

    Iq_change = Iq_A;

    printf("Iq, pos = %.2f,%d\r\n", Iq_A, ImpedancePosition);
    HAL_Delay(10);
  }
}


/**
 * @date   2022/10/4
 * @author lhx
 * @brief  impedance control ͨ�����������λ�ã�ͨ��Ԥ���ⲿ��-λ�ù�ϵ������������ֵ
 * @param  parameters definition
 * @return
 * @note   usually the special usage need to be noticed
 */
TEST test_RMDmotor_sensorlessImpedanceControl_2(TEST)
{
  int32_t targetPosition = 0;
  float   k              = 10.00;  // stiffness of impedance control  mA/degree
  float   d              = 0.55;   // impedance control ����
  float   current_command;         // stiffness of impedance control
  RMDmotor_Init();
  HAL_Delay(500);

  // ����motor��ʼλ��
  RMDmotor_setCurrentZero(COMMAND);
  HAL_Delay(10);
  RMDmotor_reset(COMMAND);
  HAL_Delay(10);
  
  RMDmotor_setCurrent(COMMAND, 0);

  
  uint32_t count = 0;
  while (1) {
    count++;
    if (count % 200 == 0) {  // ÿ2s��ת360�ȣ������迹�����㷨�Ƿ��ȶ�
      targetPosition = targetPosition + 360;
    }

	// �迹���ƿ�����
    current_command = -((angle_degree - targetPosition) * k + speed_dps * d);
	
	// ��ֹ�������󣬵���ʱ�޷�����
    if (current_command > 2000) {
      current_command = 2000;
    }
    if (current_command < -2000) {
      current_command = -2000;
    }

    RMDmotor_setCurrent(COMMAND, current_command);

    //    printf("pos-I-v = %d\\%.2f\\%d\r\n", angle_degree, current_command, speed_dps);
    HAL_Delay(10); 		//ʵ�����, 100Hz �Ŀ���Ƶ�ʲ��ܱ��ֿ����ȶ�
  }
}


#endif
