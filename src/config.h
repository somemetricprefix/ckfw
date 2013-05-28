#ifndef CKFW_SRC_CONFIG_H_
#define CKFW_SRC_CONFIG_H_

// Macro used to initialize I/O ports.
// p is the letter of the avr I/O port
// b is the nth bit of that port.
#define IO_PORT(p, b) \
{ PORT##p, DDR##p, PIN##p, b }

// Defines the number of rows in the keyboard matrix.
// Maximum possible number of rows is 256.
#define MATRIX_NUM_ROWS 4

// Defines the number of columns in the keyboard matrix.
// Maximal possible numbers of columns is 16.
#define MATRIX_NUM_COLUMNS 10

#endif // CKFW_SRC_CONFIG_H_

// Pin mapping for row pins.
#ifdef DEFINE_ROWS
  IO_PORT(B, 1),
  IO_PORT(B, 0),
  IO_PORT(B, 3),
  IO_PORT(B, 2),
#endif // DEFINE_ROWS

// Pin mapping for column pins.
#ifdef DEFINE_COLUMNS
  IO_PORT(F, 0),
  IO_PORT(F, 1),
  IO_PORT(F, 5),
  IO_PORT(F, 4),
  IO_PORT(F, 6),
  IO_PORT(F, 7),
  IO_PORT(B, 6),
  IO_PORT(B, 5),
  IO_PORT(B, 4),
  IO_PORT(D, 7),
#endif // DEFINE_COLUMNS
