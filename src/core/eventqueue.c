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

// The event queue is implemented with a ringbuffer.
static u8 in, out, count;
static struct Event event_buffer[EVENT_QUEUE_SIZE];

void EventQueueWrite(struct Event event) {
  if (count == EVENT_QUEUE_SIZE) {
    LOG_WARNING("eventqueue is full (%2u,%2u)", event.row, event.column);
    return;
  }

  event_buffer[in++] = event;
  count++;
  LOG_DEBUG("event added to queue (%2u,%2u)", event.row, event.column);

  if (in == EVENT_QUEUE_SIZE)
    in = 0;
}

struct Event EventQueueRead(void) {
  ASSERT(!EventQueueEmpty());

  struct Event event = event_buffer[out++];
  count--;

  if (out == EVENT_QUEUE_SIZE)
    out = 0;

  LOG_DEBUG("event removed from queue (%2u,%2u)", event.row, event.column);

  return event;
}

bool EventQueueEmpty(void) {
  return (count == 0);
}
