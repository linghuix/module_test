#ifndef BEIJING_DRIVER_CONTROL_FUNCTION_H
#define BEIJING_DRIVER_CONTROL_FUNCTION_H

#include "func_can.h"

extern void delay_us(uint32_t us);
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define CHANGE_BYTE (20*1.0)
#define GET_PVT_QUEUE_HEAD 0x136
#define GET_PVT_QUEUE_TAIL 0x137
#define GET_CURRENT_VALUE  0XA2
#define GET_VOLTAGE_VALUE  0XDE
#define GET_ALERT_STATE    0XC8
typedef struct Driver_info
{
	int queue_index;
	int position ;
	int speed ;
	int queue_tail_index;
	int voltage;
	int complite_sign ;
}Driver_info;

unsigned short getCRC16(unsigned char *ptr,unsigned char len);
int serial_send(int* ptr_serial,unsigned char* send_buf,int send_len);
int get_driver_real_position(unsigned char * buf);
int get_driver_set_position(unsigned char * buf);
int get_driver_pvt_index(unsigned char * buf);
int get_current_value(unsigned char * buf);
int Para_Motor_Data(unsigned char * buf,int buf_len,unsigned short command,unsigned short micro_command,int* value);
int uart_recv(int *fd,unsigned short command,unsigned short micro_command,int *value);
int motor_enable(int motor_address);
int motor_disable(int motor_address);
int motor_set_max_v(int set_speed,int motor_address);
int motor_set_max_a(int set_acceleration,int motor_address);
int motor_set_max_j(int set_jerk,int motor_address);
int motor_move_abs(double set_angle,int motor_address,int dir);
int motor_move_rel(double set_angle,int motor_address,int dir);
int motor_go(int motor_address);
int motor_stop(int motor_address);
int motor_Brake(int motor_address,uint8_t enable);
int motor_get_real_pos(int motor_address);
int motor_get_set_pos(int motor_address);
int motor_echo_test(int motor_address);
int motor_control_mode(int motor_address,unsigned char control_mode);
int motor_move_mode(int motor_address,unsigned char control_mode);
int motor_reset_enc(int motor_address);
int motor_set_pvt_data_source(int motor_address,unsigned char source);
int motor_set_pvt_control_cycle(int motor_address,unsigned short control_cycle);
int motor_set_trajectory_mode(int motor_address,unsigned short trajectory_mode);
int motor_write_data_to_pvt_queue(int motor_address,double angle,unsigned char dir);
int motor_set_Tcurve_max_a(int motor_address,short acceleration);
int motor_pvt_mode_start(int motor_address);
int motor_pvt_mode_end(int motor_address);
int motor_pvt_mode_pause(int motor_address);
int motor_get_pvt_queue_head(int motor_address);
int motor_get_pvt_queue_tail(int motor_address);
int motor_pvt_queue_clear(int motor_address);
int motor_get_current_value(int motor_address);
int motor_set_current_value(int motor_address,short current_value);
int motor_get_speed(int motor_address);
int motor_set_speed(int speed,int dir,int motor_address);
int motor_set_positive_current_limit_value(int motor_address,unsigned short percent);
int motor_set_negative_current_limit_value(int motor_address,unsigned short percent);
int motor_set_positive_current_limit_state(int motor_address,unsigned short value);
int motor_set_negative_current_limit_state(int motor_address,unsigned short value);
int motor_set_positive_current_limit_sign(int motor_address,unsigned short sign);
int motor_set_negative_current_limit_sign(int motor_address,unsigned short sign);
int motor_get_voltage_value(int motor_address);
void sxy_init_motor(int motor_mode);


/**
  * @brief  TEST
  */
TEST expriment_1(void);
#endif
