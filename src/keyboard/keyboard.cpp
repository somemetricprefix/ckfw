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

#include "core/matrix.h"
#include "core/keycodes.h"
#include "core/report.h"
#include "util/tapkey.h"

static const u8 keymap[Matrix::kNumRows][Matrix::kNumColumns] = { KEYMAP };

void Update() {
  u8 num_keys_pressed = 0;
  for (u8 row = 0; row < Matrix::kNumRows; row++) {
    for (u8 col = 0; col < Matrix::kNumColumns; col++) {
      const Key *key = Matrix::GetKey(row, col);
      if (key->Pressed()) {
        Report::AddKeycode(keymap[row][col]);
        num_keys_pressed++;
      } else if (key->Released()) {
        Report::RemoveKeycode(keymap[row][col]);
      }
    }
  }

  TapKey tap_key(Matrix::GetKey(3, 6), KC_BSPACE, KC_RSHIFT);
  tap_key.Update(num_keys_pressed);
}
