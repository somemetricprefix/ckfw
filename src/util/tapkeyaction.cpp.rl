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

#include "tapkeyaction.h"

#include "core/eventqueue.h"
#include "core/matrix.h"
#include "core/report.h"
#include "core/usb/usb.h"

%%{
  machine tapkey;
  access this->;

  action Tap {
    ReportAddKeycode(tap_keycode_);
    ReportSend();
    ReportRemoveKeycode(tap_keycode_);
  }

  action StartTimer { TimerStart(&timer_); }

  action StopTimer { TimerStop(&timer_); }

  action TimerError {
    LOG_ERROR("Unexpected timeout received. (%2u,%2u)", row_, column_);
  }

  action PressError {
    LOG_ERROR("Key press event during down state. (%2u,%2u)", row_, column_);
  }

  action ReleaseError {
    LOG_ERROR("Key release event during up state. (%2u,%2u)", row_, column_);
  }

  action SetHoldKeycodeHold { LOG_DEBUG("SetHold"); current_hold_keycode_ = hold_keycode_; }

  action SetHoldKeycodeTap { LOG_DEBUG("SetTap"); current_hold_keycode_ = tap_keycode_; }

  action AddHold { ReportAddKeycode(current_hold_keycode_); }

  action RemoveHold { ReportRemoveKeycode(current_hold_keycode_); }

  main := (
    start: ( 'p' @SetHoldKeycodeHold @StartTimer -> pressing
           | 'r' @ReleaseError -> start
           | 'o' -> start
           | 't' @TimerError -> start
           ),

    pressing: ( 'p' @PressError -> pressing
              | 'r' @Tap @StopTimer @StartTimer -> releasing
              | 'o' @AddHold @StopTimer -> holding
              | 't' @AddHold -> holding
              ),

    releasing: ( 'p' @SetHoldKeycodeTap @StopTimer @StartTimer -> pressing
               | 'r' @ReleaseError -> releasing
               | 'o' @StopTimer -> start
               | 't' -> start
               ),

    holding: ( 'p' @PressError -> holding
             | 'r' @RemoveHold -> start
             | 'o' -> holding
             | 't' @TimerError -> holding
             )
  );
}%%

%% write data;

TapKeyAction::TapKeyAction(u8 row, u8 col, u8 tap_keycode, u8 hold_keycode)
    : row_(row),
      column_(col),
      tap_keycode_(tap_keycode),
      hold_keycode_(hold_keycode),
      timer_{kTapThreshold, row, col} {
  %% write init;
}

void TapKeyAction::Execute(const Event *ev) {
  char event_character = GetEventCharacter(ev);

  LOG_DEBUG("%c", event_character);

  const char *p = &event_character;
  const char *pe = p + 1;

  %% write exec;
}

// Translate event to a character.
char TapKeyAction::GetEventCharacter(const Event *ev) const {
  const bool this_key = (ev->row == row_ && ev->column == column_);

  if (!this_key) {
    if (ev->type == kEventPressed)
      return 'o';
    else
      return '\0';
  }

  switch (ev->type) {
    case kEventPressed:
      return 'p';
    case kEventReleased:
      return 'r';
    case kEventTimeout:
      return 't';
  }

  /* NOTREACHED */
  return '\0';
}
