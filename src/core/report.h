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

#ifndef CKFW_SRC_CORE_REPORT_H_
#define CKFW_SRC_CORE_REPORT_H_

#include "common.h"

// Holds data for a HID report and methods to change keycodes in the report.
// The report format is a bitmap of 16bytes. Details can be found in
// usb/descriptors.c where the format is specified.
class Report {
 public:
  static const u8 kDataSize = 16;

  // Adds an keycode to report data.
  static void AddKeycode(u8 keycode);
  // Removes an keycode report data.
  static void RemoveKeycode(u8 keycode);

  // Returns a pointer to the data of this report.
  static void *data() { return (void *)data_; }

 private:
  // Absolute Keycodes are represented as bitfields ins the report data.
  // Example: The keycode value of 0x04 ("A" key on qwerty) is equal to the 4th
  // bit in the key bitmap. Note that there is a "0th" bit.
  //
  // 000...000x000000000000
  // \____________/\______/
  //       |          |
  //     Keys      Modifiers
  static u8 data_[kDataSize];

  static inline bool IsModifier(u8 kc) { return 0xE0 <= kc && kc <= 0xE7; }

  static void KeycodeAction(u8 keycode, bool add);
};

#endif // CKFW_SRC_CORE_REPORT_H_
