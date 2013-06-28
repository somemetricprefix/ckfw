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

static const uint kBufferSize = 8;
static struct Event buffer[kBufferSize];

// Indixes to the buffer.
static uint in = 0;
static uint out = 0;

// Number of events in the buffer.
static uint count = 0;

struct Event *EventQueueWrite() {
  if (count == kBufferSize)
    return NULL;

  struct Event *ev = &buffer[in];

  // Move to next free element.
  in = (in + 1) % kBufferSize;
  count++;

  return ev;
}

struct Event *EventQueueRead() {
  if (count == 0)
    LOG_WARNING("Eventqueue is empty.");
    return NULL;

  struct Event *ev = &buffer[out];

  // Move to next element.
  out = (out + 1) % kBufferSize;
  --count;

  return ev;
}
