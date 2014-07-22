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

#ifndef CKFW_SRC_UTIL_TAPKEY_H_
#define CKFW_SRC_UTIL_TAPKEY_H_

#include "common.h"
#include "timer.h"
#include "eventqueue.h"

// A short key press is called "tap". A tap key sends a keycode when they are
// released within a short period of time after they've been pressed. If the
// key is held down or any other key is pressed at the same time a different
// keycode is sent.
// When the key is tapped and then held down the tap keycode is sent as long
// as the key is held down.
// Example: TapKey(KC_BSPACE, KC_SHIFT) is a key that acts like a normal shift
// key in most cases. If it is tapped it acts as backspace. Tapping first and
// holding afterwards repeats the backspace as long as it is held down.

// A key press and release within kTapThreshold ms is interpreted as tap.
static const uint kTapThreshold = 200;

struct TapKey {
  // Public
  u8 row;
  u8 column;
  u8 tap_keycode;
  u8 hold_keycode;

  // Private
  Timer timer;
  u8 current_hold_keycode;
  int cs;  // Current state variable for ragel.
};

// Initialize or reset tap key to default state.
void TapKeyInit(struct TapKey *tk,
                u8 row, u8 col, u8 tap_keycode, u8 hold_keycode);

// Send event to tap key state machine.
void TapKeyExecute(struct TapKey *tk, Event event);

#endif  //  CKFW_SRC_UTIL_TAPKEY_H_
