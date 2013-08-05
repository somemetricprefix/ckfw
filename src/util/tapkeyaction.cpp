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

#include "tapkeyaction.h"

#include "core/matrix.h"
#include "core/report.h"
#include "core/usb/usb.h"

// Tapping detection is implemented in state_ machine because the logic is very
// tricky to get right without one.
void TapKeyAction::Execute(Event *ev) {
  const bool this_key = (ev->row == row_ && ev->column == column_);

  const bool pressed           = this_key && (ev->event == kEventPressed);
  const bool released          = this_key && (ev->event == kEventReleased);
  const bool timeout           = this_key && (ev->event == kEventTimeout);
  const bool other_key_pressed = !this_key && (ev->event == kEventPressed);

  switch (state_) {
    case TapStates::kStart:
      if (pressed) {
        timer_.Start();
        state_ = TapStates::kWaitTapRelease1;
      }
      break;

    case TapStates::kWaitTapRelease1:
      if (other_key_pressed || timeout) {
        report.AddKeycode(hold_keycode_);
        state_ = TapStates::kHold;
      } else if (released) {
        report.AddKeycode(tap_keycode_);
        report.Commit();
        report.RemoveKeycode(tap_keycode_);
        state_ = TapStates::kTap;
      }
      break;

    case TapStates::kTap:
      if (pressed) {
        report.AddKeycode(tap_keycode_);
        timer_.Start();
        state_ = kWaitTapRelease2;
      } else {
        timer_.Start();
        state_ = kWaitTapPress;
      }
      break;

    case TapStates::kWaitTapPress:
      if (timeout) {
        state_ = kStart;
      } else if (pressed) {
        report.AddKeycode(tap_keycode_);
        timer_.Start();
        state_ = kWaitTapRelease2;
      }
      break;

    case TapStates::kWaitTapRelease2:
      if (other_key_pressed) {
        report.RemoveKeycode(tap_keycode_);
        report.AddKeycode(hold_keycode_);
        state_ = TapStates::kHold;
      } else if (timeout) {
        state_ = TapStates::kTapHold;
      } else if (released) {
        report.RemoveKeycode(tap_keycode_);
        state_ = TapStates::kTap;
      }
      break;

    case TapStates::kTapHold:
      if (released) {
        report.RemoveKeycode(tap_keycode_);
        state_ = kStart;
      }
      break;

    case TapStates::kHold:
      if (released) {
        report.RemoveKeycode(hold_keycode_);
        state_ = kStart;
      }
      break;
  }
}
