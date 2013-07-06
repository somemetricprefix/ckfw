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

#include "ringbuffer.h"

static const uint kEventQueueSize = 8;
static Event event_buffer[kEventQueueSize];
static RingBuffer<Event> event_ring_buffer(event_buffer, kEventQueueSize);

void EventQueueWrite(u8 event, u8 row, u8 col) {
  ASSERT(event == kEventPressed ||
         event == kEventReleased ||
         event == kEventTimeout);

  bool ok = event_ring_buffer.Write({event, row, col});
  if (!ok)
    LOG_WARNING("Eventqueue is full.");
}

Event EventQueueRead() {
  return event_ring_buffer.Read();
}

bool EventQueueEmpty() {
  return event_ring_buffer.Empty();
}
