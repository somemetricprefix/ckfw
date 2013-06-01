#ifndef CKFW_SRC_CONFIG_H_
#define CKFW_SRC_CONFIG_H_

// Defines the number of rows in the keyboard matrix.
#define MATRIX_NUM_ROWS 4

// Defines the number of columns in the keyboard matrix.
#define MATRIX_NUM_COLUMNS 10

// Pin mapping for row pins.
#define MATRIX_ROW_PORTS \
  IO_PORT(B, 1), \
  IO_PORT(B, 0), \
  IO_PORT(B, 3), \
  IO_PORT(B, 2),

// Pin mapping for column pins.
#define MATRIX_COLUMN_PORTS \
  IO_PORT(F, 0), \
  IO_PORT(F, 1), \
  IO_PORT(F, 5), \
  IO_PORT(F, 4), \
  IO_PORT(F, 6), \
  IO_PORT(F, 7), \
  IO_PORT(B, 6), \
  IO_PORT(B, 5), \
  IO_PORT(B, 4), \
  IO_PORT(D, 7),

#endif // CKFW_SRC_CONFIG_H_
