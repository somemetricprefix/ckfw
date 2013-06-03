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

#ifndef CKFW_SRC_KEYBOARD_KEYMAP_H_
#define CKFW_SRC_KEYBOARD_KEYMAP_H_

#include "core/common.h"
#include "core/matrix.h"

// Holds the keymap data specified in config.h and is the bridge between
// Matrix and Report class.
class Keymap {
 public:
  // Scans matrix for key state changes and sets adds the respective keycodes
  // to the report that is send to the host.
  static void MapKeys();

 private:
  static void KeyPressed(u8 row, u8 col);
  static void KeyReleased(u8 row, u8 col);

  static const u8 keymap_[Matrix::kNumRows][Matrix::kNumColumns];
};

#endif // CKFW_SRC_KEYMAP_H_