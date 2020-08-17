/*
 * func_can.h
 *
 *  Created on: Mar 14, 2020
 *      Author: test
 */

#ifndef _FUNC_DAC_H_
#define _FUNC_DAC_H_

#include "BSP.h"

#define ChannelOne 0x000001U   	// 0000U
#define ChannelTwo 0x000020U	// 0010U

TEST Single_DAC_Output(void);
TEST Single_DAC_Output_16_to_8(void);
TEST Single_DAC_Output_8(void);
TEST Single_DAC_Tri_Output(void);
TEST Dual_DAC_Tri_Output(void);
TEST Single_DAC_Noise_Output(void);
TEST Single_DAC2_Tri_Output(void);


#endif /* BSP_FUNC_CAN_H_ */
