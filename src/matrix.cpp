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

Matrix::Key Matrix::matrix_[kNumRows][kNumColumns];

void Matrix::Init() {
    for (u8 i = 0; i < kNumColumns; i++) {
      // Configure column ports as input with pull-up.
      column_ports_[i].PullUp();
    }
};

void Matrix::Scan() {
  // Number of samples it takes to change key press state.
  // A higher number increases the debounce time.
  const u8 kDebounceSamples = 4;

  for (u8 row = 0; row < kNumRows; row++) {
    SelectRow(row);

    for (u8 col = 0; col < kNumColumns; col++) {
      // This debounce code is inspired by debounce.c written by Kenneth a. Kuhn
      // The original implementation and documentation can be found at:
      // http://www.kennethkuhn.com/electronics/debounce.c
      if (ReadColumn(col)) {
        if ((matrix_[row][col].debounce_integrator < kDebounceSamples) &&
            (++matrix_[row][col].debounce_integrator == kDebounceSamples)) {
          matrix_[row][col].pressed = 1;
        }
      } else {
        if ((matrix_[row][col].debounce_integrator > 0) &&
            (--matrix_[row][col].debounce_integrator == 0)) {
          matrix_[row][col].pressed = 0;
        }
      }
    }

    DeselectRow(row);
  }
}
