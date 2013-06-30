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
static struct KeyEvent event_pool[kEventPoolSize];

static STAILQ_HEAD(EventQueue, KeyEvent) event_queue =
  STAILQ_HEAD_INITIALIZER(event_queue);

void EventQueueWrite(Event event, u8 row, u8 col) {
  KeyEvent *key_event = NULL;

  // Search for free event in pool.
  for (uint i = 0; i < kEventPoolSize; i++) {
    if (event_pool[i].event == kFree) {
      key_event = &event_pool[i];
      break;
    }
  }

  if (!key_event) {
    LOG_WARNING("Eventpool is full.");
    return;
  }

  // Insert into queue based on priority.
  if (key_event->event < Event::kPriority)
    STAILQ_INSERT_TAIL(&event_queue, key_event, stq_entry);
  else
    STAILQ_INSERT_HEAD(&event_queue, key_event, stq_entry);

  // Write event data.
  key_event->event = event;
  key_event->row = row;
  key_event->column = col;
}

KeyEvent *EventQueueRead() {
  KeyEvent *event = STAILQ_FIRST(&event_queue);
  if (event)
    STAILQ_REMOVE_HEAD(&event_queue, stq_entry);
  return event;
}
