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

#include "core/eventqueue.h"
#include "core/matrix.h"
#include "core/report.h"
#include "util/keycodes.h"
#include "util/tapkey.h"

// BuTECK layout
// B U . , X P C L M F
// H I E A O D T R N S
// K Y Ö Ä Ü J G W V Z
static const u8 keymap[matrix::kNumRows][matrix::kNumColumns] =  {
  { KC_B, KC_U, KC_DOT, KC_COMM, KC_X, KC_P, KC_C, KC_L, KC_M, KC_F },
  { KC_H, KC_I, KC_E, KC_A, KC_O, KC_D, KC_T, KC_R, KC_N, KC_S },
  { KC_K, KC_Z, KC_SCLN, KC_QUOT, KC_LBRC, KC_J, KC_G, KC_W, KC_V, KC_Y },
  { 0, 0, 0, KC_SPC, 0, 0, 0, 0, 0, 0 },
};

static TapKey tap_keys[] = {
  { 3, 6, KC_BSPACE, KC_RSHIFT },
  { 3, 4, KC_ENTER, KC_LCTRL },
  { 3, 2, KC_TAB, KC_LALT },
};

void Tick() {
  u8 num_keys_pressed = 0;

  KeyEvent *ev;

  while ((ev = EventQueueRead())) {
    u8 keycode = keymap[ev->row][ev->column];
    if (ev->event == kPressed) {
      report::AddKeycode(keycode);
      num_keys_pressed++;
    } else if (ev->event == kReleased) {
      report::RemoveKeycode(keycode);
    }
    ev->event = kFree;
  }

  for (TapKey &tap_key : tap_keys)
    tap_key.Update(num_keys_pressed);
}
