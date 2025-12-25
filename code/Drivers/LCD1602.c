/**
 * @file LCD1602.c
 * @brief LCD1602液晶显示屏驱动
 */

#include "LCD1602.h"

/**
 * @brief 向LCD1602写入命令
 */
void lcd1602_write_cmd(uint8_t cmd) {
  LCD1602_RS = 0;          // 选择指令寄存器 (RS=0)
  LCD1602_RW = 0;          // 选择写模式 (RW=0)
  LCD1602_E = 0;           // 使能信号初始为低电平
  LCD1602_DATAPORT = cmd;  // 将命令数据输出到数据总线

  delay_ms(1);
  LCD1602_E = 1;  // 拉高使能信号，启动数据锁存
  delay_ms(1);
  LCD1602_E = 0;  // 拉低使能信号，完成数据写入
}

/**
 * @brief 向LCD1602写入数据
 */
void lcd1602_write_data(uint8_t dat) {
  LCD1602_RS = 1;
  LCD1602_RW = 0;
  LCD1602_E = 0;
  LCD1602_DATAPORT = dat;

  delay_ms(1);
  LCD1602_E = 1;
  delay_ms(1);
  LCD1602_E = 0;
}

/**
 * @brief 初始化LCD1602显示器
 */
void lcd1602_init(void) {
  lcd1602_write_cmd(0x38);
  lcd1602_write_cmd(0x0C);
  lcd1602_write_cmd(0x06);
  lcd1602_write_cmd(0x01);
}

/**
 * @brief 清除LCD1602显示内容
 */
void lcd1602_clear(void) { lcd1602_write_cmd(0x01); }

/**
 * @brief 在指定位置显示字符串
 */
void lcd1602_show_string(uint8_t x, uint8_t y, uint8_t* str) {
  uint16_t i = 0;  // 字符计数器，用于跟踪当前显示的字符位置

  // 边界检查
  if (y > 1 || x > 15) {
    return;
  }

  if (y == 0) {
    while (*str != '\0') {
      if (i < 16 - x) {
        // 计算第一行显示地址：0x80 + 当前位置
        lcd1602_write_cmd(0x80 + i + x);
      } else {
        // 超出第一行范围，换到第二行显示
        lcd1602_write_cmd(0x40 + 0x80 + i + x - 16);
      }
      lcd1602_write_data(*str++);  // 写入字符数据并移动指针
      i++;
    }
  } else {
    while (*str != '\0') {
      if (i < 16 - x) {
        // 计算第二行显示地址：0x80 + 0x40 + 当前位置
        lcd1602_write_cmd(0x80 + 0x40 + i + x);
      } else {
        // 超出第二行范围，换到第一行继续显示
        lcd1602_write_cmd(0x80 + i + x - 16);
      }
      lcd1602_write_data(*str++);
      i++;
    }
  }
}
