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

#include "matrix.h"

#include <avr/io.h>

namespace matrix {

// Each row and each column of the matrix has a dedicated IO port.
// Port definitions are in config.h.
static const IoPort row_ports[kNumRows] = { MATRIX_ROW_PORTS };
static const IoPort column_ports[kNumColumns] = { MATRIX_COLUMN_PORTS };

Key keys[kNumRows][kNumColumns];

void Init() {
    for (u8 i = 0; i < kNumColumns; i++) {
      // Configure column ports as input with pull-up.
      column_ports[i].PullUp();
    }
};

// Low signal on row pin selects the row.
static inline void SelectRow(u8 row) { row_ports[row].WriteLow(); }

// Pulls up the row pin.
static inline void DeselectRow(u8 row) { row_ports[row].PullUp(); }

// Returns true if key is pressed in that column.
static inline bool ReadColumn(u8 col) {
  // Pull-ups pins are used on input and key press pulls the input to low
  // level thus the logic has to be inverted here.
  return !column_ports[col].Read();
}

void Scan() {
  for (u8 row = 0; row < kNumRows; row++) {
    SelectRow(row);

    for (u8 col = 0; col < kNumColumns; col++) {
      const bool input = ReadColumn(col);
      keys[row][col].Debounce(input);
    }

    DeselectRow(row);
  }
}

}  // namespace matrix
