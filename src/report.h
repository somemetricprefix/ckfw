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
#include "usb/usb.h"

// The HID report is an encoded set of keycodes that is transferred over usb.
// This file defines funtions to translate keycodes into the report format.

#define REPORT_SIZE 16

void ReportKeycodeAction(u8 keycode, bool add);

static inline void ReportAddKeycode(u8 keycode) {
  if (!keycode)
    return;

  LOG_DEBUG("keycode added (0x%.2X)", keycode);

  ReportKeycodeAction(keycode, true);
}

static inline void ReportRemoveKeycode(u8 keycode) {
  if (!keycode)
    return;

  LOG_DEBUG("keycode removed (0x%.2X)", keycode);

  ReportKeycodeAction(keycode, false);
}

static inline void ReportSend(void) { UsbSendReport(true); }

bool ReportChanged(void);

u8 *ReportData(void);

#endif  // CKFW_SRC_CORE_REPORT_H_
