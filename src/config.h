#ifndef CKFW_SRC_CONFIG_H_
#define CKFW_SRC_CONFIG_H_

#include <avr/io.h>

// Pin mapping for row pins.
#define MATRIX_ROW_PORTS(PORT) \
  PORT(F, 1) \
  PORT(D, 1) \
  PORT(B, 6) \
  PORT(C, 7)

// Pin mapping for column pins.
#define MATRIX_COLUMN_PORTS(PORT) \
  PORT(B, 2) \
  PORT(B, 3) \
  PORT(B, 7) \
  PORT(D, 0) \
  PORT(B, 1) \
  PORT(D, 5) \
  PORT(D, 4) \
  PORT(F, 0) \
  PORT(F, 7) \
  PORT(F, 6) \
  PORT(F, 5) \
  PORT(F, 4)

#endif // CKFW_SRC_CONFIG_H_
