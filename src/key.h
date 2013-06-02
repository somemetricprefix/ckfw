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

#ifndef CKFW_SRC_KEY_H_
#define CKFW_SRC_KEY_H_

#include "common.h"

// Holds different states of a key.
// States are updated in cycles therefore all functions that return a key
// state are only true for the last cycle.
class Key {
 public:
  // Updates all key states according to the input on the pin for that key.
  // This method must be called every ms, so that debounce and tap timing
  // is accurate.
  inline void Update(bool input) {
    Debounce(input);
    UpdateTapState();
  };

  // A key press and release within kTapThreshold ms is interpreted as tap.
  // Maximum value is 255 because this is a unsigned 8bit value.
  static const u8 kTapThreshold = 200;

  // Returns true if a key changed from up posistin to down position, false
  // otherwise.
  inline bool Pressed() const {
    return debounce_state == DebounceStates::kPressed;
  }

  // Returns true if a key changed from up posistin to down position, false
  // otherwise.
  inline bool Released() const {
    return debounce_state == DebounceStates::kReleased;
  }

  // Returns true if key is held down, false otherwise.
  inline bool IsDown() const {
    return down;
  }

  // Returns true if a key tap was detected, false otherwise. A keytap is
  // always detected when a key was pressed and released within a short period
  // of time. Therefor a key tap is always detected in the same cycle the key is
  // released.
  inline bool Tapped() const {
    return tap_state == TapStates::kTap;
  }

 private:
  enum DebounceStates {
    kUpIdle,
    kDown1,
    kDown2,
    kPressed,
    kDownIdle,
    kUp1,
    kUp2,
    kReleased,
  };

  enum TapStates {
    kIdle,
    kWait,
    kTap,
  };

  // Updates key states if a stable state is entered.
  void Debounce(bool input);

  // Has to be called after Debounce().
  void UpdateTapState();

  unsigned debounce_state : 3;
  unsigned down : 1;
  unsigned tap_state : 2;

  // Tracks for how many cycles a key has been in down or up position.
  u8 tap_cycle;
};

#endif // CKFW_SRC_KEY_H_
