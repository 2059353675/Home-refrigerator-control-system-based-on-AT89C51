/**
 * @file main.c
 * @brief 主程序：读取 DS18B20 ROM 并显示在 LCD1602 上
 */

#include <stdio.h>

#include "DEV51.H"
#include "DS18B20.H"
#include "LCD1602.H"


// 用于存储 ROM 数据
uint8_t rom_data[8];

// 用于格式化字符串
uint8_t display_buffer[17];  // 最多 16 字符 + '\0'

/**
 * @brief 读取 DS18B20 的 64 位 ROM 地址
 * @param rom_array 存储 8 字节 ROM 数据的数组
 */
void ds18b20_read_rom(uint8_t* rom_array) {
  uint8_t i;
  ds1820rst();     // 复位设备
  ds1820wr(0x33);  // 发送 Read ROM 命令 (0x33)
  for (i = 0; i < 8; i++) {
    rom_array[i] = ds1820rd();  // 依次读取 8 字节 ROM 数据
  }
}

void main(void) {
  lcd1602_init();   // 初始化 LCD1602
  lcd1602_clear();  // 清屏

  // 读取 DS18B20 的 ROM 地址
  ds18b20_read_rom(rom_data);

  // 第一行显示前4字节
  sprintf(display_buffer, "%02X%02X%02X%02X", rom_data[0], rom_data[1],
          rom_data[2], rom_data[3]);
  lcd1602_show_string(0, 0, display_buffer);

  // 第二行显示后4字节
  sprintf(display_buffer, "%02X%02X%02X%02X", rom_data[4], rom_data[5],
          rom_data[6], rom_data[7]);
  lcd1602_show_string(0, 1,
                      display_buffer);  // 假设 LCD 支持三行，或手动换行处理

  while (1);  // 死循环，等待观察显示结果
}
