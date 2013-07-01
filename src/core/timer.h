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

SLIST_HEAD(TimerList, Timer);

class Timer {
 public:
  constexpr Timer(u16 ticks, u8 row, u8 column)
      : ticks_(ticks),
      	row_(row),
      	column_(column),
        remaining_ticks_(0),
        sl_entry_() {}

  void Start();

  // Updates all timers in list. If a timer is finished a timeout event is
  // added to the eventqueue.
  static void Update();

 private:
  const u16 ticks_;
  const u8 row_;
  const u8 column_;
  u16 remaining_ticks_;
  SLIST_ENTRY(Timer) sl_entry_;

  static TimerList timer_list_;

  DISALLOW_COPY_AND_ASSIGN(Timer);
};

#endif  // CKFW_SRC_CORE_TIMER_H_
