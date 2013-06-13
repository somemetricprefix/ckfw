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

#ifndef CKFW_SRC_CORE_MATRIX_H_
#define CKFW_SRC_CORE_MATRIX_H_

#include "common.h"
#include "config.h"

namespace matrix {

#define AS_STRUCT(port, ddr, pin, bit, id) u8 id;
struct __NumRows { MATRIX_ROW_PORTS(AS_STRUCT) };
struct __NumColumns { MATRIX_COLUMN_PORTS(AS_STRUCT) };

const u8 kNumRows = sizeof(__NumRows);
const u8 kNumColumns = sizeof(__NumColumns);

extern u8 key_matrix[kNumRows][kNumColumns];

// Returns true if a key changed from up posistin to down position, false
// otherwise.
inline bool KeyPressed(u8 row, u8 col) {
  return BIT_IS_SET(key_matrix[row][col], 7);
}

// Returns true if a key changed from up posistin to down position, false
// otherwise.
inline bool KeyReleased(u8 row, u8 col) {
  return BIT_IS_SET(key_matrix[row][col], 6);
}

// Initializes the I/O ports of the keys.
void Init();

// Updates the keys with the current key states.
void Update();

}  // namespace matrix

#endif  // CKFW_SRC_CORE_MATRIX_H_
