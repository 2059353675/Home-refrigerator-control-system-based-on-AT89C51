#include "DEV51.H"

void delay_us(uint16_t us) { while (us--); }

void delay_10us(uint16_t tens_us) {
  uint16_t i;
  for (i = 0; i < tens_us; i++) {
    delay_us(10);
  }
}

void delay_ms(uint16_t ms) {
  unsigned int i, j;
  for (i = 0; i < ms; i++) {
    for (j = 0; j < 123; j++);
  }
}