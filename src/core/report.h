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
// This file conains functions to translate keycodes to the format that
// is specified by the HID report descriptor.
namespace report {

const u8 kDataSize = 16;
extern u8 data[kDataSize];

// Sets the keycode state in the report.
void KeycodeAction(u8 keycode, bool add);

// Adds an keycode to report data.
inline void AddKeycode(u8 keycode)
{
  if (!keycode)
    return;

  KeycodeAction(keycode, true);
}

// Removes an keycode report data.
inline void RemoveKeycode(u8 keycode)
{
  if (!keycode)
    return;

  KeycodeAction(keycode, false);
}

}  // namespace report

#endif  // CKFW_SRC_CORE_REPORT_H_
