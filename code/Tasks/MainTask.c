/**
 * @file MainTask.c
 * @author Zheng Yujun
 */

#include <RTX51TNY.H>

#include "DEV51.H"
#include "DS18B20.H"
#include "KEY.H"
#include "LCD1602.H"

// 任务定义
#define TASK_ID_INIT 0
#define TASK_ID_TEMP_READ 1
#define TASK_ID_DISPLAY 2
#define TASK_ID_COMPRESSOR 3
#define TASK_ID_BUTTON 4
#define TASK_ID_DOOR_MONITOR 5

// 引脚定义
sbit COMPRESSOR_REFRIG_PIN = P1 ^ 4;  // 冷藏室继电器
sbit COMPRESSOR_FREEZE_PIN = P1 ^ 5;  // 冷冻室继电器
sbit DOOR_SENSOR_PIN = P0 ^ 4;        // 冰箱门状态检测引脚
sbit BUZZER_PIN = P0 ^ 5;             // 蜂鸣器引脚
sbit WARNING_LED_PIN = P1 ^ 6;        // 警告灯引脚

// 传感器 ROM ID
const uint8_t rom_id_refrig[8] = {0x28, 0x30, 0xC5, 0xB8,
                                  0x22, 0x00, 0x00, 0x55};
const uint8_t rom_id_freeze[8] = {0x28, 0x30, 0xC5, 0xB8,
                                  0x33, 0x00, 0x00, 0xB4};

// 阈值与目标温度超过该值则启动继电器
#define THRESHOLD 5

// 全局变量
int16_t temp_refrig;             // 当前冷藏室温度
int16_t temp_freeze;             // 当前冷冻室温度
int16_t set_temp_refrig = 40;    // 冷藏室目标温度初始值，单位为 0.1°C
int16_t set_temp_freeze = -180;  // 冷冻室目标温度初始值
bit display_update_flag = 0;     // 触发显示更新标志位
bit door_timeout_flag = 0;       // 冰箱门常开标志位

/**
 * @brief 将温度值格式化为字符串
 * @param temp 温度值，单位为 0.1°C
 * @param str 输出字符串缓冲区
 */
void format_temp_string(int16_t temp, uint8_t* str) {
  uint8_t int_part, dec_part;
  uint8_t is_negative = 0;
  uint8_t idx = 0;
  uint16_t temp_abs;

  if (temp < 0) {
    is_negative = 1;
    temp_abs = -temp;
  } else {
    temp_abs = temp;
  }

  int_part = temp_abs / 10;
  dec_part = temp_abs % 10;

  if (is_negative) {
    str[idx++] = '-';
  } else {
    str[idx++] = ' ';
  }

  // 处理十位与个位数字
  if (int_part >= 10) {
    str[idx++] = (int_part / 10) + '0';
    str[idx++] = (int_part % 10) + '0';
  } else {
    str[idx++] = ' ';
    str[idx++] = (int_part % 10) + '0';
  }

  str[idx++] = '.';
  str[idx++] = dec_part + '0';
  //   str[idx++] = 'C';
  str[idx] = '\0';
}

/**
 * @brief 初始化任务
 */
void task_init(void) _task_ TASK_ID_INIT {
  os_create_task(TASK_ID_TEMP_READ);
  os_create_task(TASK_ID_DISPLAY);
  os_create_task(TASK_ID_COMPRESSOR);
  os_create_task(TASK_ID_BUTTON);
  os_create_task(TASK_ID_DOOR_MONITOR);

  os_delete_task(TASK_ID_INIT);
}

/**
 * @brief 温度读取任务
 */
void task_temp_read(void) _task_ TASK_ID_TEMP_READ {
  while (1) {
    temp_refrig = get_temper(rom_id_refrig);
    temp_freeze = get_temper(rom_id_freeze);
    display_update_flag = 1;

    os_wait(K_TMO, 100, 0);
  }
}

/**
 * @brief 显示更新任务
 */
void task_display(void) _task_ TASK_ID_DISPLAY {
  uint8_t temp_str[16];

  lcd1602_init();
  lcd1602_clear();
  lcd1602_show_string(0, 0, "Ref:");
  lcd1602_show_string(0, 1, "Fre:");

  while (1) {
    if (display_update_flag) {
      // 显示冷藏室当前温度和设定温度
      format_temp_string(temp_refrig, temp_str);
      lcd1602_show_string(4, 0, temp_str);
      format_temp_string(set_temp_refrig, temp_str);
      lcd1602_show_string(11, 0, temp_str);

      // 显示冷冻室当前温度和设定温度
      format_temp_string(temp_freeze, temp_str);
      lcd1602_show_string(4, 1, temp_str);
      format_temp_string(set_temp_freeze, temp_str);
      lcd1602_show_string(11, 1, temp_str);

      display_update_flag = 0;
    }

    os_wait(K_TMO, 50, 0);
  }
}

/**
 * @brief 压缩机控制任务
 */
void task_compressor(void) _task_ TASK_ID_COMPRESSOR {
  bit refriger_on = 0;  // 记录压缩机当前状态
  bit freeze_on = 0;

  COMPRESSOR_REFRIG_PIN = 0;
  COMPRESSOR_FREEZE_PIN = 0;

  while (1) {
    if (door_timeout_flag) {
      COMPRESSOR_REFRIG_PIN = 0;
      COMPRESSOR_FREEZE_PIN = 0;
      refriger_on = 0;
      freeze_on = 0;
    } else {
      // 冰箱压缩机控制
      if (!refriger_on && (temp_refrig > set_temp_refrig + THRESHOLD)) {
        refriger_on = 1;
        COMPRESSOR_REFRIG_PIN = 1;
      } else if (refriger_on && (temp_refrig < set_temp_refrig - THRESHOLD)) {
        refriger_on = 0;
        COMPRESSOR_REFRIG_PIN = 0;
      }

      // 冷冻室压缩机控制
      if (!freeze_on && (temp_freeze > set_temp_freeze + THRESHOLD)) {
        freeze_on = 1;
        COMPRESSOR_FREEZE_PIN = 1;
      } else if (freeze_on && (temp_freeze < set_temp_freeze - THRESHOLD)) {
        freeze_on = 0;
        COMPRESSOR_FREEZE_PIN = 0;
      }
    }

    os_wait(K_TMO, 20, 0);
  }
}

/**
 * @brief 任务4：按键处理任务
 */
void task_button(void) _task_ TASK_ID_BUTTON {
  uint8_t key;

  while (1) {
    key = key_scan(1);  // 连续扫描模式

    switch (key) {
      case KEY_1_PRESS:
        // +0.1°C 冷藏室
				if (set_temp_refrig < 100) {
					set_temp_refrig += 1;
					display_update_flag = 1;
				}
        break;

      case KEY_2_PRESS:
        // -0.1°C 冷藏室
				if (set_temp_refrig > 0) {
					set_temp_refrig -= 1;
					display_update_flag = 1;
        }
				break;

      case KEY_3_PRESS:
        // +0.1°C 冷冻室
				if (set_temp_freeze < -80) {
					set_temp_freeze += 1;
					display_update_flag = 1;
				}
        break;

      case KEY_4_PRESS:
        // -0.1°C 冷冻室
				if (set_temp_freeze > -220) {
					set_temp_freeze -= 1;
					display_update_flag = 1;
				}
        break;

      case KEY_UNPRESS:
        break;
    }

    os_wait(K_TMO, 1, 0);
  }
}

/**
 * @brief 任务5：冰箱门状态监控任务
 */
void task_door_monitor(void) _task_ TASK_ID_DOOR_MONITOR {
  uint16_t door_open_time = 0;  // 门开启时间计数（单位：100ms）
  WARNING_LED_PIN = 0;

  while (1) {
    if (DOOR_SENSOR_PIN) {
      // 门打开（高电平），每 100ms 增加一次
      door_open_time++;

      // 超过1分钟（600 x 100ms = 60秒）
      if (door_open_time > 600) {
        BUZZER_PIN = 1;
        WARNING_LED_PIN = 1;
      }

      // 超过10分钟
      if (door_open_time > 6000) {
        door_timeout_flag = 1;
      }
    } else {
      // 门关闭，重置计时器和报警设备
      door_open_time = 0;
      BUZZER_PIN = 0;
      WARNING_LED_PIN = 0;
      door_timeout_flag = 0;
    }

    os_wait(K_TMO, 10, 0);
  }
}
