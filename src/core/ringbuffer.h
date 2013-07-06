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

#ifndef CKFW_SRC_CORE_RINGBUFFER_H_
#define CKFW_SRC_CORE_RINGBUFFER_H_

template <typename T>
class RingBuffer {
 public:
  constexpr RingBuffer(T *buffer, uint size)
      : buffer_(buffer),
        size_(size),
        in_(0),
        out_(0),
        count_(0) {}

  // Writes an new element to the buffer.
  // Returns true if element was written, false if buffer is full.
  inline bool Write(T elem) {
    if (count_ == size_)
      return false;

    // Copy element to buffer and update indecies and count.
    buffer_[in_] = elem;
    in_ = (in_ + 1) % size_;
    count_++;

    return true;
  }

  inline T Read() {
    ASSERT(!Empty());

    uint old_out = out_;
    out_ = (out_ + 1) % size_;
    --count_;
    return buffer_[old_out];
  }

  inline bool Empty() {
    return (count_ == 0);
  }

 private:
  T *buffer_;
  uint size_;

  uint in_;
  uint out_;
  uint count_;

  DISALLOW_COPY_AND_ASSIGN(RingBuffer);
};

#endif  // CKFW_SRC_CORE_RINGBUFFER_H_
