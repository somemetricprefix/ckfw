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

#include "tapkey.h"

#include "core/matrix.h"
#include "core/report.h"
#include "core/usb/usb.h"

// Tapping detection is implemented in state machine because the logic is very
// tricky to get right without one.
void TapKey::Update(bool other_key_pressed) {
  const bool timeout = (usb::frame_number - start_frame) > kTapThreshold;

  switch (state) {
    case TapStates::kStart:
      if (matrix::KeyDown(row_, column_)) {
        start_frame = usb::frame_number;
        state = TapStates::kWaitTapRelease1;
      }
      break;

    case TapStates::kWaitTapRelease1:
      if (other_key_pressed || timeout) {
        report::AddKeycode(hold_keycode_);
        state = TapStates::kHold;
      } else if (!matrix::KeyDown(row_, column_)) {
        report::AddKeycode(tap_keycode_);
        state = TapStates::kTap;
      }
      break;

    case TapStates::kTap:
      report::RemoveKeycode(tap_keycode_);
      if (matrix::KeyDown(row_, column_)) {
        report::AddKeycode(tap_keycode_);
        start_frame = usb::frame_number;
        state = kWaitTapRelease2;
      } else {
        start_frame = usb::frame_number;
        state = kWaitTapPress;
      }
      break;

    case TapStates::kWaitTapPress:
      if (timeout) {
        state = kStart;
      } else if (matrix::KeyDown(row_, column_)) {
        report::AddKeycode(tap_keycode_);
        start_frame = usb::frame_number;
        state = kWaitTapRelease2;
      }
      break;

    case TapStates::kWaitTapRelease2:
      if (other_key_pressed) {
        report::RemoveKeycode(tap_keycode_);
        report::AddKeycode(hold_keycode_);
        state = TapStates::kHold;
      } else if (timeout) {
        state = TapStates::kTapHold;
      } else if (!matrix::KeyDown(row_, column_)) {
        state = TapStates::kTap;
      }
      break;

    case TapStates::kTapHold:
      if (!matrix::KeyDown(row_, column_)) {
        report::RemoveKeycode(tap_keycode_);
        state = kStart;
      }
      break;

    case TapStates::kHold:
      if (!matrix::KeyDown(row_, column_)) {
        report::RemoveKeycode(hold_keycode_);
        state = kStart;
      }
      break;
  }
}
