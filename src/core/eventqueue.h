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

#include "common.h"

// This file defines a event queue implemented with a ring buffer.
// Events are added by interrupt routines.

enum EventType {
  kNone,
  kPressed,
  kReleased,
  kTimeout,
};

struct Event {
  EventType type;
  u8 row;
  u8 column;
};

inline void WriteEvent(Event *event, EventType type, u8 row, u8 column) {
  event->type = type;
  event->row = row;
  event->column = column;
}

// Returns a pointer to Event struct that can be written to or nullptr if queue
// is full.
struct Event *EventQueueWrite();

// Returns a poitner to Event struct taht can be read from or nullptr if queue
// is empty.
struct Event *EventQueueRead();

#endif  // CKFW_SRC_CORE_EVENTQUEUE_H_
