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

#ifndef CKFW_SRC_CORE_EVENTQUEUE_H_
#define CKFW_SRC_CORE_EVENTQUEUE_H_

#include "poolallocator.h"
#include "common.h"
#include "queue.h"

// This file defines a event queue implemented with a ring buffer.
// Events are added by interrupt routines.
// TODO: the whole event system lacks proper abstraction

enum {
  // Those events are generated in by matrix.cpp
  kEventPressed,
  kEventReleased,

  // This isn't really an event but more a seperator.
  // Every event above this has an low priority, every event below a high
  // priority.
  kEventPriority,

  kEventNumKeysPressed,
  kEventNumKeysReleased,
  kEventTimeout,
};

struct Event {
  u8 event;

  union {
    u16 num_keys;
    struct {
      u8 row;
      u8 column;
    };
  };

  inline void Free() { events.Free(this); }

  static const uint kEventPoolSize = 8;
  static PoolAllocator<Event, kEventPoolSize> events;

  STAILQ_ENTRY(Event) stq_entry;
};

// Add an event to the event queue.
void EventQueueWriteKeyEvent(u8 event, u8 row, u8 col);
void EventQueueWriteNumKeysEvent(u8 event, uint num_keys_pressed);

// Returns a poitner to Event struct that can be read from.
Event *EventQueueRead();

#endif  // CKFW_SRC_CORE_EVENTQUEUE_H_
