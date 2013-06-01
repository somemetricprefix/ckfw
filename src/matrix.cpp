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
  for (u8 row = 0; row < kNumRows; row++) {
    SelectRow(row);

    for (u8 col = 0; col < kNumColumns; col++) {
      Debounce(row, col);
    }

    DeselectRow(row);
  }
}

// Debounce routine is implemented with a state machine that models an three
// step integrator. This function was originally based on code written by
// Kenneth a. Kuhn that can be found at:
// http://www.kennethkuhn.com/electronics/debounce.c
void Matrix::Debounce(u8 row, u8 col) {
  const bool input = ReadColumn(col);
  Key &key = matrix_[row][col];

  switch (key.debounce_state) {
    case kUpIdle:
      if (input)
        key.debounce_state = kDown1;
      break;

    case kDown1:
      if (input)
        key.debounce_state = kDown2;
      else
        key.debounce_state = kUpIdle;
      break;

    case kDown2:
      if (input) {
        key.pressed = true;
        key.down = true;
        key.debounce_state = kDown3;
      } else {
        key.debounce_state = kDown1;
      }
      break;

    case kDown3:
      key.pressed = false;
      if (input)
        key.debounce_state = kDownIdle;
      else
        key.debounce_state = kDown2;
      break;

    case kDownIdle:
      if (!input)
        key.debounce_state = kUp1;
      break;

    case kUp1:
      if (input)
        key.debounce_state = kUpIdle;
      else
        key.debounce_state = kUp2;
      break;

    case kUp2:
      if (input) {
        key.debounce_state = kUp1;
      } else {
        key.released = true;
        key.down = false;
        key.debounce_state = kUp3;
      }
      break;

    case kUp3:
      key.released = false;
      if (input)
        key.debounce_state = kUp2;
      else
        key.debounce_state = kUpIdle;
      break;
  }
}
