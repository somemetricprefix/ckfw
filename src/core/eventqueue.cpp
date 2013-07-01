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

#include "poolallocator.h"

PoolAllocator<Event, Event::kEventPoolSize> Event::events;

static STAILQ_HEAD(EventQueue, Event) event_queue =
  STAILQ_HEAD_INITIALIZER(event_queue);

static Event *Enqueue(u8 event) {
  Event *ev = Event::events.Allocate();
  if (!ev) {
    LOG_WARNING("Eventpool is full.");
    return nullptr;
  }

  // Insert into queue based on priority.
  if (event < kEventPriority)
    STAILQ_INSERT_TAIL(&event_queue, ev, stq_entry);
  else
    STAILQ_INSERT_HEAD(&event_queue, ev, stq_entry);

  ev->event = event;

  return ev;
}

void EventQueueWriteKeyEvent(u8 event, u8 row, u8 col) {
  ASSERT(event == kEventPressed ||
         event == kEventReleased ||
         event == kEventTimeout);

  Event *ev = Enqueue(event);
  if (!ev)
    return;

  ev->row = row;
  ev->column = col;
}

void EventQueueWriteNumKeysEvent(u8 event, uint num_keys) {
  ASSERT(event == kEventNumKeysPressed ||
         event == kEventNumKeysReleased);

  Event *ev = Enqueue(event);
  if (!ev)
    return;

  ev->num_keys = num_keys;
}

Event *EventQueueRead() {
  Event *ev = STAILQ_FIRST(&event_queue);
  if (ev)
    STAILQ_REMOVE_HEAD(&event_queue, stq_entry);

  return ev;
}
