/**
 * @file DS18B20.c
 * @brief DS18B20 温度传感器驱动
 */

#include "DS18B20.h"

void ds1820rst(void) {
  TR0 = 0;        // 禁用定时器
  DQ = 1;         // 拉高总线
  delay_us(4);    // 延时4μs
  DQ = 0;         // 拉低总线，开始复位脉冲
  delay_us(100);  // 保持拉低
  DQ = 1;         // 释放总线
  delay_us(40);   // 等待脉冲
  TR0 = 1;        // 重新启用定时器
}

uint8_t ds1820rd(void) {
  uint8_t i, dat = 0;
  TR0 = 0;

  for (i = 0; i < 8; i++) {
    DQ = 0;  // 拉低总线
    dat >>= 1;
    DQ = 1;         // 释放总线
    if (DQ)         // 读取总线状态
      dat |= 0x80;  // 如果为高电平，将MSB设为1
    delay_us(10);   // 等待时隙结束
  }

  TR0 = 1;
  return dat;
}

void ds1820wr(uint8_t wdata) {
  uint8_t i;
  TR0 = 0;

  for (i = 0; i < 8; i++) {
    DQ = 0;
    DQ = wdata & 0x01;
    delay_us(10);
    DQ = 1;
    wdata >>= 1;
  }

  TR0 = 1;
}

int16_t get_temper(uint8_t* rom_id) {
  uint8_t a, b;     // 存储温度数据的高低字节
  uint16_t tvalue;  // 合并后的温度原始值
  int16_t itemp;    // 最终返回的温度值
  int32_t temp_calc;
  uint8_t i, j;

  // 启动温度转换
  ds1820rst();               // 复位
  ds1820wr(0x55);            // 匹配ROM
  for (i = 0; i < 8; i++) {  // 发送8字节ROM ID
    ds1820wr(rom_id[i]);
  }
  ds1820wr(0x44);

  // 读取温度数据
  ds1820rst();               // 复位
  ds1820wr(0x55);            // 匹配ROM
  for (j = 0; j < 8; j++) {  // 发送8字节ROM ID
    ds1820wr(rom_id[j]);
  }
  ds1820wr(0xBE);

  a = ds1820rd();
  b = ds1820rd();
  tvalue = ((uint16_t)b << 8) | a;  // 合并高低字节

  // 转换为0.1℃精度：原始值 × 0.0625 × 10 = 原始值 × 625 ÷ 1000
  temp_calc = (int16_t)tvalue;
  temp_calc = temp_calc * 625;

  // 四舍五入
  if (temp_calc >= 0) {
    itemp = (int16_t)((temp_calc + 500) / 1000);
  } else {
    itemp = (int16_t)((temp_calc - 500) / 1000);
  }

  return itemp;
}
