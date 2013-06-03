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

#include "keymap.h"

#include "config.h"
#include "core/key.h"
#include "core/keycodes.h"
#include "core/report.h"

const u8 Keymap::keymap_[Matrix::kNumRows][Matrix::kNumColumns] = { KEYMAP };

// This function may seem redundant at first because KeyPressed and KeyReleased
// could just be called from the Matrix class. Mapping the keys to a keycode
// should be a seperated from scanning the matrix though because then it is
// possible to check for mutiple key presses.
void Keymap::MapKeys() {
  for (u8 row = 0; row < Matrix::kNumRows; row++) {
    for (u8 col = 0; col < Matrix::kNumColumns; col++) {
      const Key *key = Matrix::GetKey(row, col);
      if (key->Pressed()) {
        KeyPressed(row, col);
      } else if (key->Released()) {
        KeyReleased(row, col);
      }
    }
  }
}

void Keymap::KeyPressed(u8 row, u8 col) {
  Report::AddKeycode(keymap_[row][col]);
}

void Keymap::KeyReleased(u8 row, u8 col) {
  Report::RemoveKeycode(keymap_[row][col]);
}
