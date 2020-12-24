#include "Beijing_driver_control_function.h"

/**
  * @brief ChengTian 程天 JA085 motor controller code.
  * @hardware CAN for communication with motor controller
  * 		  UART for Debug
  * 		  HALL sensor
  *			  Encoder
  *			  Speed reducer
  * @condition 36V power; 
  * @year 2020/08/28
  */



//#define DRIVER_TEST
#define delay_us HAL_Delay
/**
  * @brief  Hardware Interface. 
  * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @param  CAN ID of the motor in the CAN internet
  * @param  CAN frame length 0~8.
  * @retval Error status. 1 error, 0 OK.
  */

CanTxMsg MotorCommand = {0};
CanRxMsg MotorState = {0};
Error CAN_Send_Msg(unsigned char* Can_Send_Buf, int motor_address, uint8_t length)
{
	Error error;
	for(uint8_t i=0;i<8;i++){
		MotorCommand.Data[i] = Can_Send_Buf[i];
	}
	
	MotorCommand.head.RTR = CAN_RTR_DATA;
	MotorCommand.head.IDE = CAN_ID_STD;
	MotorCommand.head.DLC = length;
	MotorCommand.head.StdId = motor_address;
	
	uint32_t mailbox = 0;
	error = MX_CANx_send(&hcan1, &MotorCommand, mailbox);
	return (HAL_StatusTypeDef)(error == 0);
}


#define CAN_delay 5
unsigned short getCRC16(unsigned char *ptr,unsigned char len)
{
	unsigned char i;
	unsigned short crc = 0xFFFF;
	if(len==0)
	{
		len = 1;
	}
	while(len--)
	{
		crc ^= *ptr;
		for(i=0; i<8; i++)
		{
			if(crc&1)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
		ptr++;
	}
	return(crc);
}

/*
	function: analysis the recv_buf and get the driver data

	parameter : buf --- the message that have been cut for one frame
				driver_data --- a ptr to the motor_driver_data,to save data

	return : 1 --- get data success
			 0 --- get data fail
*/
int get_driver_real_position(unsigned char * buf)
{
	int  now_value = 0;
	if(buf == NULL)
	{
		return 0;
	}
	now_value = buf[2]<<24 | buf[3]<<16 | buf[4]<<8 | buf[5];
//	printf("\n# Real Position:%d #\n",now_value);

	return now_value;
}

int get_driver_set_position(unsigned char * buf)
{
	int  now_value = 0;
	if(buf == NULL)
	{
		return 0;
	}
	now_value = buf[2]<<24 | buf[3]<<16 | buf[4]<<8 | buf[5];
//	printf("\n# Set Position:%d #\n",now_value);

	return now_value;
}
int get_driver_pvt_index(unsigned char * buf)
{
	int now_index = 0;
	if(buf == NULL)
	{
		return -1;
	}
	now_index = buf[2]<<8 | buf[3];
//	printf("\n# pvt index:%d #\n",now_index);
	return now_index;
}

int get_current_value(unsigned char * buf)
{
	short current_value = 0;
	if(buf == NULL)
	{
		return -1;
	}
	current_value = buf[2]<<8 | buf[3];
//	printf("\n# now current value:%d #\n",current_value);
	return current_value;
}



/*
	function: cut the recv_buf and get the motor data

	parameter : buf --- the message recved
				buf_len --- the len of the message

	return : null
*/
int Para_Motor_Data(unsigned char * buf,int buf_len,unsigned short command,unsigned short micro_command,int* value)
{
	int i = 0;
	unsigned short crc_recv = 0;
	unsigned short crc_check = 0;
	unsigned char real_message[1024]={0};
	int result = 0;
	if(buf == NULL || buf_len <8)
	{
		return 0;
	}
	for(i = 0 ;i < buf_len - 7; i++)
	{
		if(buf[i] == 0x01 || buf[i] == 0x02 || buf[i] == 0x03|| buf[i] == 0x04)
		{
			crc_recv = (unsigned short)(buf[i+7]<<8|buf[i+6]);
			memcpy(real_message,(unsigned char*)&buf[i],8);
			crc_check = getCRC16(real_message,6);
			if(crc_check == crc_recv && command == real_message[1])
			{
				switch(real_message[1])
				{
					case 0x04:
						*value = get_driver_set_position(real_message);
						result = 1;
					break;
					case 0x05:
						*value = get_driver_real_position(real_message);
						result = 1;
					break;
					case 0x95:
						switch(micro_command)
						{
							case 0xA2:
								*value = get_current_value(real_message);
							break;
							case 0x136:
							case 0x137:
								*value = get_driver_pvt_index(real_message);
							break;

						}
						result = 1;
					break;
					case 0x7A:
						*value = get_driver_pvt_index(real_message);
						result = 1;
					break;
					default:
						result = 0;
					break;
				}
				if(result == 1)
				{
					return result;
				}
			}

		}
	}
	return 0;
}

/**
 * [enable the motor]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:09:17+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_enable(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x15;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("Enable motor success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [disable the motor]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:13:04+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_disable(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x16;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set motor max speed]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:13:49+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    set_speed                [the max speed you want to set,unit is RPM]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_set_max_v(int set_speed,int motor_address)
{
	if(motor_address <0 || motor_address >255 || set_speed >3000 || set_speed <0)
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x2A;
	Can_Send_Buf[2] = set_speed>>8;
	Can_Send_Buf[3] = set_speed;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set motor max acceleration]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:26:24+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    set_acceleration         [the acceleration you want to set ,max is 33]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_set_max_a(int set_acceleration,int motor_address)
{
	if(motor_address <0 || motor_address >255 || set_acceleration >7000 || set_acceleration <0)
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x2B;
	Can_Send_Buf[2] = set_acceleration>>8;
	Can_Send_Buf[3] = set_acceleration;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("set motor max a success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set motor max jerk]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:27:50+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    set_jerk                 [the jerk you want to set ,max is 33]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_set_max_j(int set_jerk,int motor_address)
{
	if(motor_address <0 || motor_address >255 || set_jerk >7000 || set_jerk <0)
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x2C;
	Can_Send_Buf[2] = set_jerk>>8;
	Can_Send_Buf[3] = set_jerk;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("set motor max j success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [set the motor's abs angle,after this setting ,motor won't run ,
 *  you need send the "motor_go" command to control the motor to run]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:29:21+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    set_angle                [the angle you want to control]
 * @param    motor_address            [the motor's address]
 * @param    dir                      [(in exoskeleton)the motor's direction,1 means forward,0 means reverse]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_move_abs(double set_angle,int motor_address,int dir)
{
	if(motor_address <0 || motor_address >4)
	{
		return 0;
	}
	if(motor_address == 1 || motor_address == 3)
	{
		if(set_angle>100)
		{
			set_angle = 100;
		}
		if(set_angle<-30)
		{
			set_angle = -30;
		}
	}
	if(motor_address == 2 || motor_address == 4)
	{
		if(set_angle<0)
		{
			set_angle = 0;
		}
		if(set_angle>95)
		{
			set_angle = 95;
		}
	}
	int position_value = 0;
	position_value = floor(set_angle*CHANGE_BYTE);
	if(dir == 0)
	{
		position_value*=-1;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x28;
	Can_Send_Buf[2] = position_value>>24;
	Can_Send_Buf[3] = position_value>>16;
	Can_Send_Buf[4] = position_value>>8;
	Can_Send_Buf[5] = position_value;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("control motor to run %d abs success\n", position_value);
		return position_value;
	}
	else
	{
		return 0;
	}
}
/**
 * [set the motor's rel angle,after this setting ,motor won't run ,
 *  you need send the "motor_go" command to control the motor to run]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:29:21+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    set_angle                [the rel-angle you want to control]
 * @param    motor_address            [the motor's address]
 * @param    dir                      [(in exoskeleton)the motor's direction,1 means forward,0 means reverse]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_move_rel(double set_angle,int motor_address,int dir)
{
	if(motor_address <0 || motor_address >4)
	{
		return 0;
	}
	int position_value = 0;

	position_value = floor(set_angle*CHANGE_BYTE);
	if(dir == 0)
	{
		position_value*=-1;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x29;
	Can_Send_Buf[2] = position_value>>24;
	Can_Send_Buf[3] = position_value>>16;
	Can_Send_Buf[4] = position_value>>8;
	Can_Send_Buf[5] = position_value;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("control motor to run %d rel success\n", position_value);
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set the motor to run]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:33:08+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_go(int motor_address)
{
	if(motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x32;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("set motor to go success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set the motor to stop]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:38:51+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_stop(int motor_address)
{
	if(motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x3C;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [brake the motor]
 * @author SunXiangYu
 * @DateTime 2019-08-21T15:38:51+0800
 * @param    enable                   [1 enable the function,0 diaable the function]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_Brake(int motor_address,uint8_t enable)
{
	if(motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x1E;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = enable;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [get the motor's position(abs)]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:40:59+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_get_real_pos(int motor_address)
{
	if(motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x05;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("get motor real pos success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_get_set_pos(int motor_address)
{
	if(motor_address <0 || motor_address >255 )
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x04;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{

		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [motor communication test]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:42:43+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_echo_test(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x0E;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("send motor echo test success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set the motor's control mode]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:44:31+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @param    control_mode             [the motor control mode you want to set ,
 *                                    	0 means current control mode,
 *                                      1 means speed control,
 *                                      2 means position control]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_control_mode(int motor_address,unsigned char control_mode)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	int i = 0;
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x25;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = control_mode;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		for(i = 0;i<8;i++)
		{
//			printf("Can_Send_Buf[%d]:0x%02x\t",i,Can_Send_Buf[i]);

		}
//		printf("\r\nset motor control mode success\r\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * [set the motor move mode in internet control mode]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:48:37+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @param    control_mode             [1 means abs move mode
 *                                     0 means relative move mode]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_move_mode(int motor_address,unsigned char control_mode)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x2D;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = control_mode;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor move mode success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_set_Tcurve_max_a(int motor_address,short acceleration)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x53;
	Can_Send_Buf[4] = acceleration>>8;
	Can_Send_Buf[5] = acceleration;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor Tcurve max acceleration success\n");
		return 1;
	}
	else
	{
		return 0;
	}

}
/**
 * [reset the motor's encoder to zero]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:56:18+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_reset_enc(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x50;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\n motor enc reset success \n");
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set the pvt use mode]
 * @author SunQinglin
 * @DateTime 2018-07-10T15:51:18+0800
 * @param    ptr_serial               [description]
 * @param    motor_address            [description]
 * @param    source                   [description]
 * @return                            [description]
 */
int motor_set_pvt_data_source(int motor_address,unsigned char source)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x3B;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = source;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor pvt data source success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_set_pvt_control_cycle(int motor_address,unsigned short control_cycle)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x39;
	Can_Send_Buf[4] = control_cycle >> 8;
	Can_Send_Buf[5] = control_cycle;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor pvt control cycle success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}



int motor_set_trajectory_mode(int motor_address,unsigned short trajectory_mode)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x57;
	Can_Send_Buf[4] = trajectory_mode >> 8;
	Can_Send_Buf[5] = trajectory_mode;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor trajectory_mode success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_write_data_to_pvt_queue(int motor_address,double angle,unsigned char dir)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	if(motor_address == 1 || motor_address == 3)
	{
		if(angle<-30)
		{
			angle = -30;
		}
		else if(angle>100)
		{
			angle = 100;
		}
	}
	if(motor_address == 2 || motor_address == 4)
	{
		if(angle<0)
		{
			angle = 0;
		}
		else if(angle>100)
		{
			angle = 100;
		}
	}
	int position = floor(angle*CHANGE_BYTE);
	if(dir == 0)
	{
		position*=-1;
	}
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x7C;
	Can_Send_Buf[2] = position>>24;
	Can_Send_Buf[3] = position>>16;
	Can_Send_Buf[4] = position >> 8;
	Can_Send_Buf[5] = position;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nwrite data to pvt queue success\n");

		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_pvt_mode_start(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x3C;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x01;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor pvt start success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_pvt_mode_end(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x3C;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor pvt end success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_pvt_mode_pause(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x3C;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x02;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor pvt pause success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_get_pvt_queue_head(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{
		return -1;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x95;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x36;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

int motor_get_pvt_queue_tail(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x95;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x37;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_pvt_queue_clear(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x35;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x01;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nclear pvt queue success success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_get_current_value(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x95;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0xA2;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nsend get motor current value success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [motor_set_current_value description]
 * @author SunQinglin
 * @DateTime 2018-07-13T15:48:24+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @param    current_value            [the current you want to set in current-control mode,unit to mA]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
int motor_set_current_value(int motor_address,short current_value)
{
	if(motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x70;
	Can_Send_Buf[2] = current_value>>8;
	Can_Send_Buf[3] = current_value;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_get_speed(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{
		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x3F;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0x00;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nsend get motor speed success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int motor_set_speed(int speed,int dir,int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{
		return 0;
	}
	int real_speed = speed;
	if(dir == 0)
	{
		real_speed*=-1;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x6F;
	Can_Send_Buf[2] = real_speed>>24;
	Can_Send_Buf[3] = real_speed>>16;
	Can_Send_Buf[4] = real_speed>>8;
	Can_Send_Buf[5] = real_speed;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nsend set motor speed [%d] success\n",real_speed);
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_set_positive_current_limit_value(int motor_address,unsigned short percent)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x2E;
	Can_Send_Buf[4] = percent >> 8;
	Can_Send_Buf[5] = percent;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor percent success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_set_negative_current_limit_value(int motor_address,unsigned short percent)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x2F;
	Can_Send_Buf[4] = percent >> 8;
	Can_Send_Buf[5] = percent;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor percent success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_set_positive_current_limit_state(int motor_address,unsigned short value)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x30;
	Can_Send_Buf[4] = value >> 8;
	Can_Send_Buf[5] = value;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor value success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_set_negative_current_limit_state(int motor_address,unsigned short value)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x31;
	Can_Send_Buf[4] = value >> 8;
	Can_Send_Buf[5] = value;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor value success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_set_positive_current_limit_sign(int motor_address,unsigned short sign)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x32;
	Can_Send_Buf[4] = sign >> 8;
	Can_Send_Buf[5] = sign;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor sign success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_set_negative_current_limit_sign(int motor_address,unsigned short sign)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x96;
	Can_Send_Buf[2] = 0x01;
	Can_Send_Buf[3] = 0x33;
	Can_Send_Buf[4] = sign >> 8;
	Can_Send_Buf[5] = sign;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nset motor sign success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
int motor_get_voltage_value(int motor_address)
{
	if(motor_address <0 || motor_address >255)
	{

		return 0;
	}
	unsigned char Can_Send_Buf[8]={0};
	unsigned short crc_cal = 0;
	Can_Send_Buf[0] = motor_address;
	Can_Send_Buf[1] = 0x95;
	Can_Send_Buf[2] = 0x00;
	Can_Send_Buf[3] = 0xDE;
	Can_Send_Buf[4] = 0x00;
	Can_Send_Buf[5] = 0x00;
	crc_cal = getCRC16(Can_Send_Buf,6);
	Can_Send_Buf[6] = crc_cal%0x100;
	Can_Send_Buf[7] = crc_cal/0x100;
	if(CAN_Send_Msg(Can_Send_Buf,motor_address,8))
	{
//		printf("\nsend get motor voltage value success\n");
		return 1;
	}
	else
	{
		return 0;
	}
}
/**
 * [set the motor's control mode]
 * @author SunXiangYu
 * @DateTime 2018-07-10T15:44:31+0800
 * @param    ptr_serial               [the pointer to the serial's file descriptor]
 * @param    motor_address            [the motor's address]
 * @param    control_mode             [the motor control mode you want to set ,
 *                                    	0 means current control mode,
 *                                      1 means speed control,
 *                                      2 means position control]
 * @return                            [the result of sending this command ,1 means success,0 means failed]
 */
void sxy_init_motor(int motor_mode)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOD时钟
//	
//	//USART1端口配置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;; //GPIOD8与GPIOD9
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //上拉
//	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PD8,PD9
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_12);
//	GPIO_SetBits(GPIOB,GPIO_Pin_13);
//	GPIO_SetBits(GPIOB,GPIO_Pin_14);
//	GPIO_SetBits(GPIOB,GPIO_Pin_15);
//	
//	delay_us(3000);
	
	motor_control_mode(0x01,motor_mode);
//	delay_us(CAN_delay);
//	motor_control_mode(0x02,motor_mode);
//	delay_us(CAN_delay);
//	motor_control_mode(0x03,motor_mode);
//	delay_us(CAN_delay);
//	motor_control_mode(0x04,motor_mode);
	
	delay_us(CAN_delay);
	
	if(motor_mode == 0x02)
	{
		motor_move_mode(0x01,0x00);
//		delay_us(CAN_delay);
//		motor_move_mode(0x02,0x01);
//		delay_us(CAN_delay);
//		motor_move_mode(0x03,0x01);
//		delay_us(CAN_delay);
//		motor_move_mode(0x04,0x01);
		
		delay_us(CAN_delay);
	}
	
	motor_set_trajectory_mode(0x01,0x00);
//	delay_us(CAN_delay);
//	motor_set_trajectory_mode(0x02,0x00);
//	delay_us(CAN_delay);
//	motor_set_trajectory_mode(0x03,0x00);
//	delay_us(CAN_delay);
//	motor_set_trajectory_mode(0x04,0x00);
//	
	delay_us(CAN_delay);

	
	motor_enable(0x01);
	delay_us(CAN_delay);
//	delay_us(CAN_delay);
//	motor_enable(0x02);
//	delay_us(CAN_delay);
//	motor_enable(0x03);
//	delay_us(CAN_delay);
//	motor_enable(0x04);
}

#ifdef DRIVER_TEST
#include "func_can.h"

//#define ADDR 0x2
uint8_t ADDR = 0x1;
#define CURRENT_MODE 0
TEST expriment_1(void)
{
//	int error;
		
	MX_CAN1_Init(CAN_MODE_NORMAL);
	CAN_Start(&hcan1);
	
	
	if(motor_control_mode(ADDR, CURRENT_MODE) == 0){
		ERROR(2, "mode set");
	}
	HAL_Delay(20);
	if(motor_set_max_v(1000, ADDR) == 0){
		ERROR(2, "v set");
	}
	HAL_Delay(20);
	if(motor_set_max_a(33, ADDR) == 0){
		ERROR(2, "acc set");
	}
	HAL_Delay(20);
	if(motor_set_max_j(33, ADDR) == 0){
		ERROR(2, "jerk set");
	}
	HAL_Delay(20);
	if(motor_set_positive_current_limit_state(ADDR, 5270) == 0){
		ERROR(2, "current max set");
	}
	if(motor_enable( ADDR) == 0){
		ERROR(2, "enable");
	}
	HAL_Delay(20);
	if(motor_set_current_value(ADDR, 1000) == 0){
		ERROR(2, "current set");
	}
	HAL_Delay(20);
	
	MSG("end ID = 0x%x\r\n",ADDR);
	
	while(1){
		motor_echo_test(ADDR);
		HAL_Delay(1000);
	}
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0)>0) {
		int canerror = MX_CANx_get(&hcan1, &MotorState, CAN_RX_FIFO0);
		MSG("receive  ");
		for(uint8_t i = 0; i<8;i++)
		{
			MSG(" 0x%X\t", MotorState.Data[i]);
		}
		MSG("\r\n");
	}
}
#endif
