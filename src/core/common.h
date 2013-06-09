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
#ifndef CKFW_SRC_CORE_COMMON_H
#define CKFW_SRC_CORE_COMMON_H

// Shorthand for common types.
typedef   signed char s8;
typedef unsigned char u8;
typedef   signed int s16;
typedef unsigned int u16;
typedef   signed int sint;
typedef unsigned int uint;

// Macros for bit manipulation.
#define BIT_SET(var, bit) ((var) |=  (1 << (bit)))
#define BIT_CLR(var, bit) ((var) &= ~(1 << (bit)))
#define BIT_IS_SET(var, bit) ((var) & (1 << bit))

// If _SFR_ASM_COMPAT is set register names are constants and have to be casted.
#define REG_BIT_SET(reg, bit) BIT_SET(*(volatile u8 *)(u16)(reg), bit)
#define REG_BIT_CLR(reg, bit) BIT_CLR(*(volatile u8 *)(u16)(reg), bit)
#define REG_BIT_IS_SET(reg, bit) BIT_IS_SET(*(volatile u8 *)(u16)(reg), bit)

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <stdio.h>
extern FILE console;

#ifdef NDEBUG
#define DEBUG(M, ...)
#else
#define DEBUG(M, ...) fprintf(Usb::console(), "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#endif // CKFW_SRC_CORE_COMMON_H
