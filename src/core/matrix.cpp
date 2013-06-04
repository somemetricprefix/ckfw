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

// This tell avr-libc to use constant definitions for IO names
#define _SFR_ASM_COMPAT 1
#include <avr/io.h>

const IoPort Matrix::row_ports_[Matrix::kNumRows] = {
  MATRIX_ROW_PORTS
};

const IoPort Matrix::column_ports_[Matrix::kNumColumns] = {
  MATRIX_COLUMN_PORTS
};

Key Matrix::matrix_[kNumRows][kNumColumns];

void Matrix::Init() {
    for (u8 i = 0; i < kNumColumns; i++) {
      // Configure column ports as input with pull-up.
      column_ports_[i].PullUp();
    }
};

void Matrix::Scan() {
  for (u8 row = 0; row < kNumRows; row++) {
    SelectRow(row);

    for (u8 col = 0; col < kNumColumns; col++) {
      const bool input = ReadColumn(col);
      matrix_[row][col].Debounce(input);
    }

    DeselectRow(row);
  }
}
