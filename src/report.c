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
// defines the report data as bitmap of 16 bytes. The first byte contains the
// modifier bitmap and the remaining 15 bytes are mapped to keycodes.
//
// Example:
// The keycode value of 0x04/'A' is equal to the 5th bit in the second byte of
// the report (1 shifted 4 times left). Keycode 0x09/'F' would be the second bit
// of the 3rd byte.
//
// Byte    1    |   2    |   3    |  4-16
// Bits 00000000|00000000|00000000|000..000
//      \______/ \________________________/
//         |                |
//      Modifiers          Keys

static u8 report_data[REPORT_SIZE];

static bool changed = false;

static inline bool IsModifier(u8 kc) { return 0xE0 <= kc && kc <= 0xE7; }

void ReportKeycodeAction(u8 keycode, bool add) {
  u8 index;

  if (IsModifier(keycode)) {
    index = 0;
  } else {
    // Add one to skip the modifier byte.
    index = 1 + (keycode / 8);
  }

  u8 nth_bit = (keycode % 8);

  if (add)
    BIT_SET(report_data[index], nth_bit);
  else
    BIT_CLR(report_data[index], nth_bit);

  changed = true;
}

bool ReportChanged(void) {
  return changed;
}

u8 *ReportData(void) {
  changed = false;
  return report_data;
}
