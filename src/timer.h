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

// This struct represents a fixed timer. Because the timer generates an event
// after timeout it needs to be bound to a key as identification. When the
// TimerStart() function is called the timer is inserted into the active timer
// list. After the timer finished it is removed from this list and a timer event
// is generated.
// The timer must not be deallocated while it’s active.
typedef struct timer {
  // Timeout in milli seconds.
  u16 ticks;

  // Key coordinates the timer is bound to.
  u8 row;
  u8 column;

  u16 remaining_ticks;
  SLIST_ENTRY(timer) sl_entry;
} Timer;

// Initializes timer.
void TimerInit(Timer *timer, u16 ticks, u8 row, u8 column);

// Starts a timer by attaching it to the timer List.
void TimerStart(Timer *timer);

// Updates all timers in list. This is called every millisecond in the core.
// Should not be called by any user code.
void TimersUpdate(void);

#endif  // CKFW_SRC_CORE_TIMER_H_
