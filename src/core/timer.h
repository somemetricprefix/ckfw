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

#ifndef CKFW_SRC_CORE_TIMER_H_
#define CKFW_SRC_CORE_TIMER_H_

#include "common.h"
#include "queue.h"

// Create list type for timers.
SLIST_HEAD(TimerList, Timer);

// This class represents a fixed timer. Each timer is bound to a specific key in
// the matrix. When Start() is called the timer is inserted into the active
// timer list. When the timer timed out it is removed from the list and a timer
// event is generated.
// The timer must not be freed while it’s active.
class Timer {
 public:
  constexpr Timer(u16 ticks, u8 row, u8 column)
      : ticks_(ticks),
        row_(row),
        column_(column),
        remaining_ticks_(0),
        sl_entry_() {}

  // Starts the timer by attaching it to the timer List.
  void Start();

  // Updates all timers in list. Must be called every millisecond.
  static void Update();

 private:
  // Timeout in milli seconds.
  const u16 ticks_;

  // Key coordinates the timer is bound to.
  const u8 row_;
  const u8 column_;

  u16 remaining_ticks_;
  SLIST_ENTRY(Timer) sl_entry_;

  // Contains all active timers.
  static TimerList timer_list_;

  DISALLOW_COPY_AND_ASSIGN(Timer);
};

#endif  // CKFW_SRC_CORE_TIMER_H_
