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

#ifndef CKFW_SRC_CORE_KEY_H_
#define CKFW_SRC_CORE_KEY_H_

#include "common.h"

// Holds different states of a key.
// States are updated in cycles therefore all functions that return a key
// state are only true for the last cycle.
class Key {
 public:
  // Updates key state when input is stable.
  void Debounce(bool input);

  // Returns true if a key changed from up posistin to down position, false
  // otherwise.
  inline bool Pressed() const {
    return debounce_state_ == DebounceStates::kPressed;
  }

  // Returns true if a key changed from up posistin to down position, false
  // otherwise.
  inline bool Released() const {
    return debounce_state_ == DebounceStates::kReleased;
  }

  // Returns true if key is held down, false otherwise.
  inline bool IsDown() const {
    return (debounce_state_ == kDownIdle) ||
           (debounce_state_ == kUp1) ||
           (debounce_state_ == kUp2) ||
           (debounce_state_ == kPressed);
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

  u8 debounce_state_;
};

#endif // CKFW_SRC_CORE_KEY_H_
