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

// Create list type for timers.
SLIST_HEAD(TimerList, Timer);

// Contains all active timers.
static struct TimerList timer_list = SLIST_HEAD_INITIALIZER(timer_list);

void TimerStart(struct Timer *timer) {
  // Only insert timer to list if it’s not active already.
  if (timer->remaining_ticks == 0) {
    SLIST_INSERT_HEAD(&timer_list, timer, sl_entry);
    LOG_DEBUG("timer started (%2u,%2u)", timer->row, timer->column);
  } else {
    LOG_WARNING("timer restarted while active (%2u,%2u)",
                timer->row, timer->column);
  }

  timer->remaining_ticks = timer->ticks;
}

void TimerStop(struct Timer *timer) {
  timer->remaining_ticks = 0;
  SLIST_REMOVE(&timer_list, timer, Timer, sl_entry);
  LOG_DEBUG("timer stopped (%2u,%2u)", timer->row, timer->column);
}

void TimersUpdate(void) {
  struct Timer *iter, *save;

  // Safe iteration so that timers can be removed.
  SLIST_FOREACH_SAFE(iter, &timer_list, sl_entry, save) {
    if (--iter->remaining_ticks <= 0) {
      EventQueueWrite((struct Event){kEventTimeout, iter->row, iter->column});
      SLIST_REMOVE(&timer_list, iter, Timer, sl_entry);
      LOG_DEBUG("timeout (%2u,%2u)", iter->row, iter->column);
    }
  }
}
