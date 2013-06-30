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

static const uint kEventPoolSize = 8;
static struct Event event_pool[kEventPoolSize];

static STAILQ_HEAD(EventQueue, Event) event_queue =
  STAILQ_HEAD_INITIALIZER(event_queue);

static Event *Enqueue(u8 ev) {
  Event *event = NULL;

  // Search for free event in pool.
  for (uint i = 0; i < kEventPoolSize; i++) {
    if (event_pool[i].event == kEventFree) {
      event = &event_pool[i];
      break;
    }
  }

  if (!event) {
    LOG_WARNING("Eventpool is full.");
    return nullptr;
  }

  // Insert into queue based on priority.
  if (ev < kEventPriority)
    STAILQ_INSERT_TAIL(&event_queue, event, stq_entry);
  else
    STAILQ_INSERT_HEAD(&event_queue, event, stq_entry);

  event->event = ev;

  return event;
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

void EventQueueWriteNumKeysPressedEvent(uint num_keys_pressed) {
  Event *ev = Enqueue(kEventNumKeysPressed);
  if (!ev)
    return;

  ev->num_keys_pressed = num_keys_pressed;
}

Event *EventQueueRead() {
  Event *event = STAILQ_FIRST(&event_queue);
  if (event)
    STAILQ_REMOVE_HEAD(&event_queue, stq_entry);
  return event;
}
