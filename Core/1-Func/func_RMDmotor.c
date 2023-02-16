/**
 *
 *  Created on: 9 30, 2022
 *  Author: lhx
 *
 *  @hardware RMD系列电机 / STM32F103 开发板 / 24V /5V
 *  @brief  使用嵌入式设备通过Motech驱动器控制程天的JA90电机模组，实现摩擦力补偿。
 *  @ref D:\整理\5-专业知识\0-硬件\100-硬件资料\2-驱动器\1-脉塔智能电机\RMD系列
 */

#include "func_RMDmotor.h"

#define COMMAND_GET_CURRENT 0x40
#define COMMAND_GET_SPEED 0x3F
#define COMMAND_GET_POS 0x05

#define RMD_COMMAND_SIZE 13


/**
 * @date   2022/9/30
 * @author glh
 * @brief  该函数用于将指令的后面两个字节加上crc校验码
 * @param
        array_of_databytes - 待添加CRC校验码的指令。
        number_of_databytes-2个字节的crc需要插入的位置，0表示从第一个字节插入crc
        commands - CRC插入后的指令. 传入全局变量 commands[13];
 * @return
 */
#define POLYNOMIAL 0xA001
#define PRESET_VALUE 0xFFFF
void CRC16func(uint8_t array_of_databytes[], uint8_t number_of_databytes, uint8_t commands[])
{
  uint16_t current_crc_value;
  uint8_t  hi, lo;
  int      i, j;
  current_crc_value = PRESET_VALUE;  //本处#define PRESET_VALUE 0xFFFF
  for (i = 0; i < number_of_databytes; i++) {
    current_crc_value = current_crc_value ^ ((unsigned int)array_of_databytes[i]);
    for (j = 0; j < 8; j++) {
      if (current_crc_value & 0x0001) {
        current_crc_value =
            (current_crc_value >> 1) ^ POLYNOMIAL;  //本处#define POLYNOMIAL 0xA001 还有其他如0x8408
                                                    //// x^16 + x^12 + x^5 + 1
      }
      else {
        current_crc_value = (current_crc_value >> 1);
      }
    }
  }
  //有的校验方法，要求对结果取反，有，不需要，视情况而定
  // current_crc_value = ~current_crc_value;
  //调换一下高低位，有的算法需要调换一下，有的不需要
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
  __HAL_UART_ENABLE_IT(&RMD_huart, UART_IT_IDLE);  // 空闲中断
}

/**
 * @date   2022/9/30
 * @author lhx
 * @brief  电流闭环控制
 * @param
        commands - command has 13 bytes
        iqcontrol_mA - 需要控制的电流大小,单位 mA
 * @return
 * @note   电机会返回 温度[int8_t 1℃/LSB]，Iq[int16_t 0.01A/LSB]，速度[int16_t
 1dps/LSB]和角度信息[int16_t 1degree/LSB]
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
                maxSpeed - speed limitation of motor (unit in 1dps/LSB). 限制了电机输出轴
转动的最大速度
                Angle_degree - (degree/LSB)

 * @return
 * @note   即发送完运动距离以后，需要发送“开始运动”指令，电机才会开始运动；
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
 * @brief  重启reset motor
 * @param
        commands - command has 13 bytes
        maxSpeed - speed limitation of motor (unit in 1dps/LSB). 限制了电机输出轴
转动的最大速度
        Angle_degree - (degree/LSB)

 * @return
 * @note   即发送完运动距离以后，需要发送“开始运动”指令，电机才会开始运动；
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
 * @brief  设置Motor的编码器数值为零
 * @param
        commands - command has 13 bytes
        maxSpeed - speed limitation of motor (unit in 1dps/LSB). 限制了电机输出轴
转动的最大速度
        Angle_degree - (degree/LSB)

 * @return
 * @note   电机重启后才有效
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
int     RMDmotor_idleflag       = 0;    // 空闲中断flag

int8_t  RMDmotor_temperature;  // 摄氏度
int16_t speed_dps;             // 电机速度值 dps
float   Iq_A;                  // 电机实际q轴电流 A
int16_t angle_degree;          // 电机实际编码器数值，degree
int32_t acceleration_dpss;     // 电机实际加速度 1 dps/s

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
 * @brief : USART3_RX_CallBack.  RMDmotor_idleflag 中的串口数据接收
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
 * @brief  admittance control 通过q轴电流估计外部力，通过预定外部力-位置关系，解算电机位置
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
 * @brief  impedance control 通过编码器检测位置，通过预定外部力-位置关系，解算电机电流值
 * @param  parameters definition
 * @return
 * @note   usually the special usage need to be noticed
 */
TEST test_RMDmotor_sensorlessImpedanceControl_2(TEST)
{
  int32_t targetPosition = 0;
  float   k              = 10.00;  // stiffness of impedance control  mA/degree
  float   d              = 0.55;   // impedance control 阻尼
  float   current_command;         // stiffness of impedance control
  RMDmotor_Init();
  HAL_Delay(500);

  // 设置motor初始位置
  RMDmotor_setCurrentZero(COMMAND);
  HAL_Delay(10);
  RMDmotor_reset(COMMAND);
  HAL_Delay(10);
  
  RMDmotor_setCurrent(COMMAND, 0);

  
  uint32_t count = 0;
  while (1) {
    count++;
    if (count % 200 == 0) {  // 每2s旋转360度，测试阻抗控制算法是否稳定
      targetPosition = targetPosition + 360;
    }

	// 阻抗控制控制量
    current_command = -((angle_degree - targetPosition) * k + speed_dps * d);
	
	// 防止电流过大，调试时无法控制
    if (current_command > 2000) {
      current_command = 2000;
    }
    if (current_command < -2000) {
      current_command = -2000;
    }

    RMDmotor_setCurrent(COMMAND, current_command);

    //    printf("pos-I-v = %d\\%.2f\\%d\r\n", angle_degree, current_command, speed_dps);
    HAL_Delay(10); 		//实验表明, 100Hz 的控制频率才能保持控制稳定
  }
}


#endif
