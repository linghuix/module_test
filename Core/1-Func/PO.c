
/*
 * PO.c
 *
 *  Created on: Jun 3, 2020
 *      Author: test
 */

#include "PO.h"
#define PI 3.1415
#define poTest(...)


/**
  * @brief  模仿math.atan2(y,x)的-pi-+pi的坐标角度算法
  *
  * @param  y xy坐标轴上的y坐标
  *
  * @param  x xy坐标轴上的x坐标
  *
  * @retval v - xy坐标轴上,坐标（x,y)到原点的直线与x轴正向的夹角弧度值，范围为-pi-+pi
*/
float myatan2(float y, float x)
{
    float v=0;
    if( y==0 && x==0){
        ERROR(2,"error y = 0, x = 0");
        return 0.0;
	}
    if( x > 0){
        v = atan(y/x);
	}
    if (y>=0 && x<0){
        v = PI + atan(y/x);
	}
    if (y<0 && x<0){
        v = -PI + atan(y/x);
	}
    if (y>0 && x==0){
        v = PI/2;
	}
    if (y<0 && x==0){
        v = -PI/2;
	}
    return v;
}


/**
 * author lhx
 * Jan 5, 2021
 *
 * @brief : PO 相位计算
 * @param d - 人体关节角度
 * @param w - 人体关节角速度
 * @return   相位. 以输入数据的最小值为零起始点
 */
float PO_phase(float d, float w)
{
	return myatan2(w, d);
}



/**
 * author lhx
 * Jun 3, 2020
 *
 * @brief : 助力系数非线性调节，以降低抖动和提高安全
 * @param ang - 人体关节角度
 * @param w - 人体关节角速度
 * @param k - 助力系数 pointer
 * @return   none
 * Window > Preferences > C/C++ > Editor > Templates.
 */
#define floatabs(...) __VA_ARGS__>0? __VA_ARGS__:-1*__VA_ARGS__
#define AssisTor 0.1	//助力矩大小
#define RightTorRatio 3	// 右侧的assist gain 
#define D_area 2.0		// for eliminate chattering
#define W_area 1.0
#define MAX_D_area 50.0	// for safety
//阈值防抖动算法
void th_algori(float ang, float w, float * k)
{
	/* 设定阈值，防止在关节角度较小的时候，助力系数k随着关节角速度的正负变化而产生抖动。 */
	if(floatabs(ang) < D_area || floatabs(w) < W_area || floatabs(ang) > MAX_D_area){
		*k = 0.0;
	}
	else{
		*k = AssisTor;
	}
	poTest("\r\nw=%.2f.&&&*k=%.2f\r\n",w,*k);
}

/**
 * author lhx
 * April 2, 2021
 *
 * @brief : 相位振荡器助力值计算算法
 * @param d - 人体关节角度
 * @param w - 人体关节角速度
 * @param node -电机序号，2为右侧电机
 * @param k - 助力系数
 * @return   助力力矩大小和方向
 */
float PO(float d, float w,uint8_t node)
{
	float sin_fai = w/sqrt(d*d+w*w);
	float k;

	th_algori(d,w,&k);
	if(node == 2){						// Right Torque assive
		k = k*RightTorRatio;
	}
	float assistive_torque = sin_fai*k;
	return assistive_torque;
}

