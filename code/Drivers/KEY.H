/**
 * @file KEY.H
 * @brief 51单片机独立按键扫描
 */

#ifndef __KEY_H__
#define __KEY_H__

#include "DEV51.H"

/**
 * @brief 独立按键硬件接口定义
 * @note 采用上拉电阻连接方式
 */
sbit KEY_1 = P0 ^ 0;
sbit KEY_2 = P0 ^ 1;
sbit KEY_3 = P0 ^ 2;
sbit KEY_4 = P0 ^ 3;

/**
 * @brief 按键状态宏定义
 */
#define KEY_1_PRESS 1  // 按键1被按下
#define KEY_2_PRESS 2  // 按键2被按下
#define KEY_3_PRESS 3  // 按键3被按下
#define KEY_4_PRESS 4  // 按键4被按下
#define KEY_UNPRESS 0  // 无按键被按下

/**
 * @brief 按键扫描函数
 * @return 按键状态
 */
uint8_t key_scan(uint8_t mode);

#endif  // __KEY_H__