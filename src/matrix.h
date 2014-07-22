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

// Macro magic to determine the number of row and port pins.
#define ID(port, bit) kPort##port##bit
#define AS_STRUCT(port, bit) u8 ID(port, bit);
struct __NumRows { MATRIX_ROW_PORTS(AS_STRUCT) };
struct __NumColumns { MATRIX_COLUMN_PORTS(AS_STRUCT) };

#define MATRIX_NUM_ROWS (sizeof(struct __NumRows))
#define MATRIX_NUM_COLUMNS (sizeof(struct __NumColumns))

// Initializes the I/O ports of the keys.
void MatrixInit(void);

// Updates the keys with the current key states.
void MatrixUpdate(void);

#endif  // CKFW_SRC_CORE_MATRIX_H_
