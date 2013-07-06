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

// The HID report is an encoded set of keycodes that is transferred over usb.
// This class represents a report and has methods to translate keycodes to the
// format that is specified by the HID report descriptor.
class Report {
 public:
  static const u8 kDataSize = 16;

  constexpr Report() : data_{0} {}

  // Adds an keycode to report.
  inline void AddKeycode(u8 keycode) {
    if (!keycode)
      return;

    LOG_DEBUG("+keycode\t\t%.2X", keycode);

    KeycodeAction(keycode, true);
  }

  // Removes an keycode from report.
  inline void RemoveKeycode(u8 keycode) {
    if (!keycode)
      return;

    LOG_DEBUG("-keycode\t\t%.2X", keycode);

    KeycodeAction(keycode, false);
  }

  // Pass the record data to the usb code for sending.
  void Commit();

  u8 *data() { return data_; }

 private:
  // Adds or removes a keycode from report.
  void KeycodeAction(u8 keycode, bool add);

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
  u8 data_[kDataSize];

  DISALLOW_COPY_AND_ASSIGN(Report);
};

extern Report report;

#endif  // CKFW_SRC_CORE_REPORT_H_
