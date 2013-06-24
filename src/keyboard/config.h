#ifndef CKFW_SRC_CONFIG_H_
#define CKFW_SRC_CONFIG_H_

#include <avr/io.h>

// Enable Teensy onboard LED.
#define ERROR() \
do { \
  DDRD |= (1 << 6); \
  PORTD |= (1 << 6); \
} while (0);

// Defines the number of rows in the keyboard matrix.
#define MATRIX_NUM_ROWS 4

// Defines the number of columns in the keyboard matrix.
#define MATRIX_NUM_COLUMNS 10

// Pin mapping for row pins.
#define MATRIX_ROW_PORTS(PORT) \
  PORT(B, 0) \
  PORT(B, 1) \
  PORT(B, 2) \
  PORT(B, 3)

// Pin mapping for column pins.
#define MATRIX_COLUMN_PORTS(PORT) \
  PORT(F, 0) \
  PORT(F, 1) \
  PORT(F, 4) \
  PORT(F, 5) \
  PORT(F, 6) \
  PORT(F, 7) \
  PORT(B, 6) \
  PORT(B, 5) \
  PORT(B, 4) \
  PORT(D, 7)

#endif // CKFW_SRC_CONFIG_H_
