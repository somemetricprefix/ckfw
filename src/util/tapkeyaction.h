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

#ifndef CKFW_SRC_UTIL_TAPKEYACTION_H_
#define CKFW_SRC_UTIL_TAPKEYACTION_H_

#include "core/common.h"
#include "keyactioninterface.h"

// A short key press is calles "tap". Tap keys send a keycode when they are
// released shortly after they've been pressed. If the key is held or anther
// key is pressed at the same time it sends another keycode.
// When the key is tapped and then held down the tap keycode is sent as long
// as the key is held down.
// Example: TapKey(KC_BSPACE, KC_SHIFT) is key that acts like a normal shift
// key in most cases. If it is tapped it acts as backspace though.
class TapKeyAction : public KeyActionInterface {
 public:
  // A key press and release within kTapThreshold ms is interpreted as tap.
  // Maximum value is 255 because this is a unsigned 8bit value.
  static const uint kTapThreshold = 200;

  constexpr TapKeyAction(u8 row, u8 col, u8 tap_keycode, u8 hold_keycode)
      : row_(row),
        column_(col),
        tap_keycode_(tap_keycode),
        hold_keycode_(hold_keycode),
        state_(kStart) {}

  virtual void Execute(Event *event);

 private:
  enum TapStates {
    kStart,
    kWaitTapRelease1,
    kTap,
    kWaitTapPress,
    kWaitTapRelease2,
    kTapHold,
    kHold,
  };

  const u8 row_;
  const u8 column_;
  const u8 tap_keycode_;
  const u8 hold_keycode_;

  u8 state_;

  DISALLOW_COPY_AND_ASSIGN(TapKeyAction);
};

#endif  //  CKFW_SRC_UTIL_TAPKEYACTION_H_
