/**
 * @file KEY.C
 * @brief 51单片机独立按键扫描实现文件
 *
 * 实现独立按键的扫描与消抖处理
 * 支持单次触发模式和连续触发模式
 */

#include "KEY.H"

uint8_t key_scan(uint8_t mode) {
  static bit key_state = 1;  // 按键状态机状态：1=可检测按下，0=已按下或正在消抖

  if (mode) {
    // 连续触发模式：每次调用都允许重新检测按键
    key_state = 1;
  }

  if (key_state == 1 &&
      (KEY_1 == 0 || KEY_2 == 0 || KEY_3 == 0 || KEY_4 == 0)) {
    // 有任意按键被按下
    delay_10us(100);

    key_state = 0;  // 锁定状态，防止重复触发

    if (KEY_1 == 0)
      return KEY_1_PRESS;
    else if (KEY_2 == 0)
      return KEY_2_PRESS;
    else if (KEY_3 == 0)
      return KEY_3_PRESS;
    else if (KEY_4 == 0)
      return KEY_4_PRESS;
  }

  else if (KEY_1 == 1 && KEY_2 == 1 && KEY_3 == 1 && KEY_4 == 1) {
    // 所有按键都释放，则恢复检测状态
    key_state = 1;
  }

  return KEY_UNPRESS;
}
