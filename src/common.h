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

// This header contains usefull typedefs and macros which are used throughout
// the whole projet.
#ifndef COMMON_H
#define COMMON_H

// Shorthand for common types.
typedef unsigned char u8;
typedef unsigned int u16;

// Macros for bit manipulation.
#define BIT_SET(var, bit) ((var) |=  (1 << (bit)))
#define BIT_CLR(var, bit) ((var) &= ~(1 << (bit)))
#define BIT_IS_SET(var, bit) ((var & (1 << bit)) >> bit)

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// TODO: make it usefull
#define ASSERT(cond) \
  if (!cond) { \
    DDRB |= (1 << 6); \
    PORTB |= (1 << 6); \
  }

#endif // COMMON_H
