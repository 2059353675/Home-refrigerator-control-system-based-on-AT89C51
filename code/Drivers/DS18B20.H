/**
 * @file DS18B20.H
 * @brief DS18B20 温度传感器驱动
 */

#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <intrins.h>

#include "DEV51.H"

sbit DQ = P1 ^ 3;  // DS18B20 数据引脚

/**
 * @brief DS18B20复位时序函数
 */
void ds1820rst(void);

/**
 * @brief 从DS18B20读取一个字节数据
 */
uint8_t ds1820rd(void);

/**
 * @brief 向DS18B20写入一个字节数据
 * @param wdata 要写入的8位数据
 */
void ds1820wr(uint8_t wdata);

/**
 * @brief 获取DS18B20的温度值
 * @param rom_id 指向8字节ROM ID的指针
 * @return 温度值，单位为0.1℃（例如：235表示23.5℃，-52表示-5.2℃）
 */
int16_t get_temper(uint8_t* rom_id);

#endif /* __DS18B20_H__ */
