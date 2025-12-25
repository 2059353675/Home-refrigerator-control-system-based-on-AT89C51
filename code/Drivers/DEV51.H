/**
 * @file DEV51.H
 * @brief 51单片机开发工具库
 *
 * 8051单片机寄存器定义、C99标准整数类型定义、精确延时函数等
 */

#ifndef __DEV51_H__
#define __DEV51_H__

#include <intrins.h>
#include <reg51.h>

// 定义C99标准整数类型
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int32_t;

/**
 * @brief 微秒级精确延时函数
 * @param us 需要延时的微秒数，范围：1-65535
 */
void delay_us(uint16_t us);

/**
 * @brief 10微秒级精确延时函数
 * @param tens_us 需要延时的10微秒数量
 */
void delay_10us(uint16_t tens_us);

/**
 * @brief 毫秒级延时函数
 * @param ms 需要延时的毫秒数，范围：1-65535毫秒
 */
void delay_ms(uint16_t ms);

#endif  //__DEV51_H__
