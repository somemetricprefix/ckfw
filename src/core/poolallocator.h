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

#ifndef CKFW_SRC_CORE_POOLALLOCATOR_
#define CKFW_SRC_CORE_POOLALLOCATOR_

#include "common.h"

template <typename T, uint size>
class PoolAllocator {
 public:
  constexpr PoolAllocator() : flags_(0) {
    static_assert(size <= sizeof(uint) * 8, "Pool allocator size too big.");
  }

  inline T *Allocate() {
    // Search for free event in pool.
    for (uint i = 0; i < size; i++) {
      if (!BIT_IS_SET(flags_, i)) {
        BIT_SET(flags_, i);
        return &pool_[i];
      }
    }

    return NULL;
  }

  inline void Free(T *ptr) {
    ASSERT(ptr >= &pool_[0] && ptr < &pool_[size]);

    uint bit = (ptr - &pool_[0]);
    BIT_CLR(flags_, bit);
  }

 private:
  T pool_[size];
  uint flags_;

  DISALLOW_COPY_AND_ASSIGN(PoolAllocator);
};

#endif  // CKFW_SRC_CORE_POOLALLOCATOR_
