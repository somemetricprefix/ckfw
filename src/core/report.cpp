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

// The report format is defined at HID descriptor level. This implemtation
// asserts report data to be a bitmap of 16 bytes. The first byte contains the
// modifier bitmal. The bits of the remaining 15 bytes are mapped to a keycode.
//
// Example:
// The keycode value of 0x04/'A' is equal to the 4th bit in the second byte of
// the report. Keycode 0x09/'F' would be the first bit of the 3rd byte.
//
// Byte    1    |   2    |   3    |  4-16
// Bits 00000000|0000a000|00000000|000..000
//      \______/ \________________________/
//         |                |
//      Modifiers          Keys

namespace report {

u8 data[report::kDataSize] = { 0 };

static inline bool IsModifier(u8 kc) { return 0xE0 <= kc && kc <= 0xE7; }

static void KeycodeAction(u8 keycode, bool add) {
  u8 data_index;

  if (IsModifier(keycode)) {
    data_index = 0;
  } else {
    // Adding one skips the Modifier byte.
    data_index = 1 + (keycode / 8);
  }

  u8 nth_bit = (keycode % 8);

  if (add)
    BIT_SET(data[data_index], nth_bit);
  else
    BIT_CLR(data[data_index], nth_bit);
}

void AddKeycode(u8 keycode)
{
  if (!keycode)
    return;

  DEBUG("+keycode\t\t%.2X", keycode);

  KeycodeAction(keycode, true);
}

void RemoveKeycode(u8 keycode)
{
  if (!keycode)
    return;

  DEBUG("-keycode\t\t%.2X", keycode);

  KeycodeAction(keycode, false);
}

}  // namespace report
