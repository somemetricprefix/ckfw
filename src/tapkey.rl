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

#include "tapkey.h"

#include "eventqueue.h"
#include "matrix.h"
#include "report.h"
#include "usb/usb.h"

%%{
  machine tapkey;
  access tk->;

  action Tap {
    ReportAddKeycode(tk->tap_keycode);
    ReportSend();
    ReportRemoveKeycode(tk->tap_keycode);
  }

  action StartTimer { TimerStart(&tk->timer); }

  action PressError {
    LOG_ERROR("Key press event during down state. (%2u,%2u)",
              tk->row, tk->column);
  }

  action ReleaseError {
    LOG_ERROR("Key release event during up state. (%2u,%2u)",
              tk->row, tk->column);
  }

  action SetHoldKeycodeHold { tk->current_hold_keycode = tk->hold_keycode; }

  action SetHoldKeycodeTap { tk->current_hold_keycode = tk->tap_keycode; }

  action AddHold { ReportAddKeycode(tk->current_hold_keycode); }

  action RemoveHold { ReportRemoveKeycode(tk->current_hold_keycode); }

  main := (
    start: ( 'p' @SetHoldKeycodeHold @StartTimer -> pressing
           | 'r' @ReleaseError -> start
           | 'x' -> start
           ),

    pressing: ( 'p' @PressError -> pressing
              | 'r' @Tap @StartTimer -> releasing
              | 'x' @AddHold -> holding
              ),

    releasing: ( 'p' @SetHoldKeycodeTap @StartTimer -> pressing
               | 'r' @ReleaseError -> releasing
               | 'x' -> start
               ),

    holding: ( 'p' @PressError -> holding
             | 'r' @RemoveHold -> start
             | 'x' -> holding
             )
  );
}%%

%% write data;

// Translate event to a character.
static char GetEventCharacter(Event ev, bool this_key) {
  if (!this_key) {
    return (ev.type == kEventPressed) ? 'x' : '\0';
  }

  switch (ev.type) {
    case kEventPressed:
      return 'p';
    case kEventReleased:
      return 'r';
    case kEventTimeout:
      return 'x';
  }

  /* NOTREACHED */
  return '\0';
}

void TapKeyInit(struct TapKey *tk, u8 row, u8 column, u8 tap_keycode,
                u8 hold_keycode) {
  tk->row = row;
  tk->column = column;
  tk->tap_keycode = tap_keycode;
  tk->hold_keycode = hold_keycode;

  TimerInit(&tk->timer, TAP_TIME, row, column);

  %% write init;
}

void TapKeyExecute(struct TapKey *tk, Event ev) {
  const bool this_key = (ev.row == tk->row && ev.column == tk->column);
  char event_character = GetEventCharacter(ev, this_key);
  if (event_character == '\0') {
    return;
  }

  const char *p = &event_character;
  const char *pe = p + 1;

  %% write exec;
}
