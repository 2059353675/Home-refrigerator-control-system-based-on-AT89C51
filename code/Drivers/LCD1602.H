/**
 * @file LCD1602.H
 * @brief LCD1602液晶显示屏驱动库
 */

#ifndef _LCD1602_H
#define _LCD1602_H

#include "DEV51.H"

sbit LCD1602_RS = P1 ^ 0;    // 寄存器选择信号 (RS): 0=指令寄存器, 1=数据寄存器
sbit LCD1602_RW = P1 ^ 1;    // 读写选择信号 (RW): 0=写操作, 1=读操作
sbit LCD1602_E = P1 ^ 2;     // 使能信号 (E): 高电平有效，用于锁存数据
#define LCD1602_DATAPORT P2  // 8位数据传输端口

/**
 * @brief LCD1602初始化函数
 */
void lcd1602_init(void);

/**
 * @brief LCD1602清屏函数
 */
void lcd1602_clear(void);

/**
 * @brief 在指定位置显示字符串
 * @param x 显示起始列坐标 (0-15)，表示字符位置(0-15)
 * @param y 显示起始行坐标 (0-1)，0表示第一行，1表示第二行
 * @param str 要显示的字符串指针，以'\0'结尾
 */
void lcd1602_show_string(uint8_t x, uint8_t y, uint8_t* str);

#endif  // _LCD1602_H
