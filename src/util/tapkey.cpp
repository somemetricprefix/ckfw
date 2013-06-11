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

#include "core/report.h"

// Tapping detection is implemented in state machine because the logic is very
// tricky to get right without one.
void TapKey::Update(bool other_key_pressed) {
  const bool timeout = (cycle == kTapThreshold);

  switch (state) {
    case TapStates::kStart:
      if (key_->Pressed()) {
        report::AddKeycode(hold_keycode_);
        cycle = 0;
        state = TapStates::kWaitTapRelease1;
      }
      break;

    case TapStates::kWaitTapRelease1:
      if (other_key_pressed || timeout) {
        state = TapStates::kHold;
      } else if (key_->Released()) {
        report::RemoveKeycode(hold_keycode_);
        report::AddKeycode(tap_keycode_);
        state = TapStates::kTap;
      } else {
        cycle++;
      }
      break;

    case TapStates::kTap:
      report::RemoveKeycode(tap_keycode_);
      if (key_->Pressed()) {
        report::AddKeycode(tap_keycode_);
        cycle = 0;
        state = kWaitTapRelease2;
      } else {
        cycle = 0;
        state = kWaitTapPress;
      }
      break;

    case TapStates::kWaitTapPress:
      if (timeout) {
        state = kStart;
      } else if (key_->Pressed()) {
        report::AddKeycode(tap_keycode_);
        cycle = 0;
        state = kWaitTapRelease2;
      } else {
        cycle++;
      }
      break;

    case TapStates::kWaitTapRelease2:
      if (other_key_pressed) {
        report::RemoveKeycode(tap_keycode_);
        report::AddKeycode(hold_keycode_);
        state = TapStates::kHold;
      } else if (timeout) {
        state = TapStates::kTapHold;
      } else if (key_->Released()) {
        state = TapStates::kTap;
      } else {
        cycle++;
      }
      break;

    case TapStates::kTapHold:
      if (key_->Released()) {
        report::RemoveKeycode(tap_keycode_);
        state = kStart;
      }
      break;

    case TapStates::kHold:
      if (key_->Released()) {
        report::RemoveKeycode(hold_keycode_);
        state = kStart;
      }
      break;
  }
}
