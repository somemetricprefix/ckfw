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

#include "matrix.h"

#include <avr/io.h>

#include "eventqueue.h"

namespace matrix {

struct OutputPort {
  const u8 port;
  const u8 ddr;
  const u8 bit;
};

#define AS_DATA(port, bit) { (u16)&PORT##port, (u16)&DDR##port, bit },
static const OutputPort row_ports[kNumRows] = { MATRIX_ROW_PORTS(AS_DATA) };

// Each key is represented as an 8-bit value.
// It is a bitmask of the last states of that key.
u8 key_matrix[kNumRows][kNumColumns];

// Pull up on IO port is enabled by setting DDRx = 0 and PORTx = 1.
#define AS_PULLUP(port, bit) \
  BIT_CLR(DDR##port, bit); \
  BIT_SET(PORT##port, bit);

void Init() {
  // Configure column ports as input with pull-up.
  MATRIX_COLUMN_PORTS(AS_PULLUP);
};

// Selects row by setting output to low level.
static inline void SelectRow(u8 row) {
  BIT_SET(*(volatile u8 *)(u16)row_ports[row].ddr, row_ports[row].bit);
  BIT_CLR(*(volatile u8 *)(u16)row_ports[row].port, row_ports[row].bit);
}

// Deselct row by enabling internal pull up resistor.
static inline void DeselectRow(u8 row) {
  BIT_CLR(*(volatile u8 *)(u16)row_ports[row].ddr, row_ports[row].bit);
  BIT_SET(*(volatile u8 *)(u16)row_ports[row].port, row_ports[row].bit);
}

// Translates to id to a index. Used as "static counter".
#define AS_ENUM(port, bit) ID(port, bit),
enum { MATRIX_COLUMN_PORTS(AS_ENUM) };

// Pull-ups pins are used on input and key press pulls the input to low
// level thus the logic has to be inverted here.
// Current key value is shifted left once to put the new input value in the
// first bit.
#define AS_READ_INPUT(port, bit) { \
  u8 key = key_matrix[row][ID(port, bit)]; \
  key <<= 1; \
  if (!BIT_IS_SET(PIN##port, bit)) \
    key += 1; /* += generates an inc instruction, |= generates ldi + or */ \
  key_matrix[row][ID(port, bit)] = key; \
}

void Update() {
  // First pass: scan matrix
  for (u8 row = 0; row < kNumRows; row++) {
    SelectRow(row);

    MATRIX_COLUMN_PORTS(AS_READ_INPUT);

    DeselectRow(row);
  }

  u16 num_keys_pressed = 0;
  u16 num_keys_released = 0;

  // Second pass: debounce
  // The method used here was mentioned by Soarer in a thread on geekhack:
  // http://geekhack.org/index.php?topic=42385.msg861321#msg861321
  for (u8 i = 0; i < kNumRows; i++) {
    for (u8 j = 0; j < kNumColumns; j++) {
      // Apply mask to get the 6 state bits.
      u8 key = key_matrix[i][j] & 0b00111111;

      // Check for change between 6th and 5th bit. If the key was stable for
      // 5 cycles after that set pressed or released bit.
      if (key == 0b011111) {  // 0 -> 1 indicates key press.
        LOG_DEBUG("+key\t\t%u,%u", i, j);

        EventQueueWriteKeyEvent(kEventPressed, i, j);
        num_keys_pressed++;
      } else if (key == 0b100000) {  // 1 -> 0 indicates key release.
        LOG_DEBUG("-key\t\t%u,%u", i, j);

        EventQueueWriteKeyEvent(kEventReleased, i, j);
        num_keys_released++;
      }
    }
  }

  if (num_keys_pressed)
    EventQueueWriteNumKeysEvent(kEventNumKeysPressed, num_keys_pressed);
  if (num_keys_released)
    EventQueueWriteNumKeysEvent(kEventNumKeysReleased, num_keys_released);
}

}  // namespace matrix
