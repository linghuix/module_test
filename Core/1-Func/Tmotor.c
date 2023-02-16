/**
 * @date   2022/9/26
 * @author lhx
 * @brief  程天 prototype 测试 PO 算法，
 */


#include "TMOTOR.h"

/**
 * @date   2022/09/26
 * @author lhx
 * @brief  无符号整数转浮点数据，等比例转换函数
 * @param  x_int - 带转换的无符号整数
                        x_min -
                        x_max -
                        bits -
 * @return 浮点数
 */
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
  /// converts unsigned int to float, given range and number of bits ///
  float span   = x_max - x_min;
  float offset = x_min;
  return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}

/**
 * @date   2022/09/26
 * @author lhx
 * @brief  浮点数据 转 无符号数，等比例转换函数
 * @param  x - 待转换浮点数
 * @return
 */
int float_to_uint(float x, float x_min, float x_max, int bits)
{
  /// Converts a float to an unsigned int, given range and number of bits
  float span   = x_max - x_min;
  float offset = x_min;
  return (int)((x - offset) * ((float)((1 << bits) - 1)) / span);
}


CanTxMsg tmotor_tx_msg;
/**
 * @date   2022/09/26
 * @author lhx
 * @brief  控制模式指令填装函数，标准帧，数据帧
 * @param  parameters definition
 * @return
 */
void control_command_buff(uint8_t id, float _pos, float _vel, float _KP, float _KD, float _torq)
{
  uint16_t pos_tmp, vel_tmp, kp_tmp, kd_tmp, tor_tmp;

  pos_tmp = float_to_uint(_pos, P_MIN, P_MAX, 16);
  vel_tmp = float_to_uint(_vel, V_MIN, V_MAX, 12);
  kp_tmp  = float_to_uint(_KP, KP_MIN, KP_MAX, 12);
  kd_tmp  = float_to_uint(_KD, KD_MIN, KD_MAX, 12);
  tor_tmp = float_to_uint(_torq, T_MIN, T_MAX, 12);

  tmotor_tx_msg.head.StdId = id;
  tmotor_tx_msg.head.DLC   = 8;
  tmotor_tx_msg.head.RTR   = CAN_RTR_DATA;
  tmotor_tx_msg.head.IDE   = CAN_ID_STD;

  tmotor_tx_msg.Data[0] = (pos_tmp >> 8);
  tmotor_tx_msg.Data[1] = pos_tmp;
  tmotor_tx_msg.Data[2] = (vel_tmp >> 4);
  tmotor_tx_msg.Data[3] = ((vel_tmp & 0xF) << 4) | (kp_tmp >> 8);
  tmotor_tx_msg.Data[4] = kp_tmp;
  tmotor_tx_msg.Data[5] = (kd_tmp >> 4);
  tmotor_tx_msg.Data[6] = ((kd_tmp & 0xF) << 4) | (tor_tmp >> 8);
  tmotor_tx_msg.Data[7] = tor_tmp;
}

/**
 * @date   2022/9/26
 * @author lhx
 * @brief  驱动器置零函数
 * @return
 */
void motor_encoderZeroSetting(uint8_t id)
{
  unsigned char i = 0;

  tmotor_tx_msg.head.StdId = id;
  tmotor_tx_msg.head.DLC   = 8;
  tmotor_tx_msg.head.RTR   = CAN_RTR_DATA;
  tmotor_tx_msg.head.IDE   = CAN_ID_STD;

  for (i = 0; i < 8; i++) {
    if (i == 7)
      tmotor_tx_msg.Data[i] = 0xFE;
    else
      tmotor_tx_msg.Data[i] = 0xFF;
  }

  i = 0;
  MSG_BSTART("T-Motor", "zero setting");
  while (MX_CANx_send(&hcan1, &tmotor_tx_msg, 0) != HAL_OK && i < 10) {  //发送8个字节
    HAL_Delay(50);
    i++;
  };
  if(i<10){
	MSG_ASTART("T-Motor", "zero setting");
  }
}

/**
 * @date   2022/9/28
 * @author lhx
 * @brief  进入电机
 * @param  parameters definition
 * @return
 * @note   usually the special usage need to be noticed
 */
void motorEnable(uint8_t id)
{
  uint8_t i = 0;

  tmotor_tx_msg.head.StdId = id;
  tmotor_tx_msg.head.DLC   = 8;
  tmotor_tx_msg.head.RTR   = CAN_RTR_DATA;
  tmotor_tx_msg.head.IDE   = CAN_ID_STD;

  for (i = 0; i < 8; i++) {
    if (i == 7)
      tmotor_tx_msg.Data[i] = 0xFC;
    else
      tmotor_tx_msg.Data[i] = 0xFF; 
  }

  i = 0;
  MSG_BSTART("T-Motor", "enable");
  while (MX_CANx_send(&hcan1, &tmotor_tx_msg, 0) != HAL_OK && i < 10) {  //发送8个字节
    HAL_Delay(50);
    i++;
  };
  if(i<10){
	MSG_ASTART("T-Motor", "enable");
  }
}

// -------------------*---------------------------*---------- TEST ---------*----------------------*-----------------//
// -------------------*---------------------------*---------- TEST ---------*----------------------*-----------------//


uint32_t mailbox = 0;  // can 发送邮箱编号
TEST     test_motor_torque_control(TEST)
{
  uint8_t ID = 2;

  MX_CAN1_Init(CAN_MODE_NORMAL);
  CAN_Start(&hcan1);

  motor_encoderZeroSetting(ID);  //驱动器编码器置0
  motorEnable(ID);			//使能电机

	while(1){
  control_command_buff(ID, 0, 0, 0, 0, 10);
  MX_CANx_send(&hcan1, &tmotor_tx_msg, mailbox);
		HAL_Delay(10);
	}
}

/**
 * @brief  CAN callback function in interrupt service.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

  CanRxMsg ForceData;  // raw data can frame
  if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0) {
    int canerror = MX_CANx_get(&hcan1, &ForceData, CAN_RX_FIFO0);

    for (int i = 0; i < 8; i++) {
      printf("%d - ", ForceData.Data[i]);
    }
    printf("\r\n");
  }
}
