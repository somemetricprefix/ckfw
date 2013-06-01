/*
 * Copyright (c) 2013 somemetricprefix <somemetricprefix+code@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef CKFW_SRC_MATRIX_H_
#define CKFW_SRC_MATRIX_H_

#include "common.h"
#include "config.h"
#include "ioport.h"

class Matrix {
 public:
  static const u8 kNumRows = MATRIX_NUM_ROWS;
  static const u8 kNumColumns = MATRIX_NUM_COLUMNS;

  // Initializes the I/O ports of the matrix.
  // __attribute__((optimize("-O3"))) is used because -O3 creates proper
  // instructions (sbi / cbi). That makes the code produced by -O3 smaller and
  // more efficient than the code produced by -Os.
  static void Init() __attribute__((optimize("-O3")));

  // Updates the matrix with the current key states.
  static void Scan();

  static inline bool IsKeyPressed(u8 row, u8 col) {
    return matrix_[row][col].pressed;
  }

 private:
  struct Key {
    unsigned pressed : 1;
    unsigned debounce_integrator : 7;
  };

  // Each row and column of the matrix has a dedicated IO port.
  // Port definitions are in config.h
  static const IoPort row_ports_[kNumRows];
  static const IoPort column_ports_[kNumColumns];

  // Low signal on row pin selects the row.
  static inline void SelectRow(u8 row) { row_ports_[row].WriteLow(); }

  // Pulls up the row pin.
  static inline void DeselectRow(u8 row) { row_ports_[row].PullUp(); }

  // Returns true if key is pressed in that column.
  static inline bool ReadColumn(u8 col) {
    // Pull-ups pins are used on input and key press pulls the input to low
    // level thus the logic has to be inverted here.
    return !column_ports_[col].Read();
  }

  static Key matrix_[kNumRows][kNumColumns];
};

#endif //CKFW_SRC_MATRIX_H_
