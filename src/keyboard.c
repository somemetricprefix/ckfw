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
#include "matrix.h"
#include "report.h"
#include "keycodes.h"
#include "tapkey.h"

static const u8 keymap[MATRIX_NUM_ROWS][MATRIX_NUM_COLUMNS] =  {
  { KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_NO,   KC_NO,
    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P    },
  { KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_NO,   KC_NO,
    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN },
  { KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC, KC_QUOT,
    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH },
  { KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_SPC,   KC_NO,   KC_NO,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  },
};

static struct TapKey tab_keys[3];

void Init(void) {
  TapKeyInit(&tab_keys[0], 3, 5, KC_ENTER, KC_CAPS);
  TapKeyInit(&tab_keys[1], 3, 6, KC_BSPACE, KC_RALT);
  TapKeyInit(&tab_keys[2], 3, 7, KC_BSPACE, KC_RSHIFT);
}

void KeyEvent(Event event) {
  for (u8 i = 0; i < 3; i++) {
    TapKeyExecute(&tab_keys[i], event);
  }

  u8 keycode = keymap[event.row][event.column];

  switch (event.type) {
    case kEventPressed:
      ReportAddKeycode(keycode);
      break;

    case kEventReleased:
      ReportRemoveKeycode(keycode);
      break;
  }
}
