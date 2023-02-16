/*
 * PO.h
 *
 *  Created on: Jun 3, 2020
 *      Author: test
 */

#ifndef _FUNC_TMOTOR_H_
#define _FUNC_TMOTOR_H_


#include  "func_can.h"


#define P_MIN 	-12.5
#define P_MAX 	12.5
#define V_MIN 	-45
#define V_MAX   45
#define KP_MIN	0
#define KP_MAX	500
#define KD_MIN	0
#define KD_MAX	5
#define T_MIN	-18
#define T_MAX	18

void Zero_setting(uint8_t id);
void control_command_buff(uint8_t id, float _pos, float _vel, float _KP, float _KD, float _torq);

TEST test_motor_torque_control(TEST);

#endif /* 1_FUNC_PO_H_ */
