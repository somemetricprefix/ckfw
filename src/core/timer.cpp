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

#include "eventqueue.h"
#include "timer.h"

TimerList Timer::timer_list_ = SLIST_HEAD_INITIALIZER(timer_list_);

void Timer::Update() {
  Timer *iter, *save;
  SLIST_FOREACH_SAFE(iter, &timer_list_, sl_entry_, save) {
    if (--iter->remaining_ticks_ == 0) {
      EventQueueWriteKeyEvent(kEventTimeout, iter->row_, iter->column_);
      SLIST_REMOVE(&timer_list_, iter, Timer, sl_entry_);
    }
  }
}

void Timer::Start() {
  // If time is not up it is still in the list.
  if (remaining_ticks_ == 0)
    SLIST_INSERT_HEAD(&timer_list_, this, sl_entry_);

  remaining_ticks_ = ticks_;
}
