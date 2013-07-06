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

#include "report.h"

Report report;

static inline bool IsModifier(u8 kc) { return 0xE0 <= kc && kc <= 0xE7; }

void Report::KeycodeAction(u8 keycode, bool add) {
  u8 data_index;

  if (IsModifier(keycode)) {
    data_index = 0;
  } else {
    // Adding one skips the Modifier byte.
    data_index = 1 + (keycode / 8);
  }

  u8 nth_bit = (keycode % 8);

  if (add)
    BIT_SET(data_.data[data_index], nth_bit);
  else
    BIT_CLR(data_.data[data_index], nth_bit);
}
