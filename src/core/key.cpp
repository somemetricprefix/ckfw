#include "key.h"

// Debounce routine is implemented with a state machine that models an three
// step integrator. This function was originally based on code written by
// Kenneth a. Kuhn that can be found at:
// http://www.kennethkuhn.com/electronics/debounce.c
void Key::Debounce(bool input) {
  switch (debounce_state_) {
    case DebounceStates::kUpIdle:
      if (input)
        debounce_state_ = DebounceStates::kDown1;
      break;

    case DebounceStates::kDown1:
      if (input)
        debounce_state_ = DebounceStates::kDown2;
      else
        debounce_state_ = DebounceStates::kUpIdle;
      break;

    case DebounceStates::kDown2:
      if (input) {
        down_ = true;
        debounce_state_ = DebounceStates::kPressed;
      } else {
        debounce_state_ = DebounceStates::kDown1;
      }
      break;

    case DebounceStates::kPressed:
      if (input)
        debounce_state_ = DebounceStates::kDownIdle;
      else
        debounce_state_ = DebounceStates::kUp1;
      break;

    case DebounceStates::kDownIdle:
      if (!input)
        debounce_state_ = DebounceStates::kUp1;
      break;

    case DebounceStates::kUp1:
      if (input)
        debounce_state_ = DebounceStates::kDownIdle;
      else
        debounce_state_ = DebounceStates::kUp2;
      break;

    case DebounceStates::kUp2:
      if (input) {
        debounce_state_ = DebounceStates::kUp1;
      } else {
        down_ = false;
        debounce_state_ = DebounceStates::kReleased;
      }
      break;

    case DebounceStates::kReleased:
      if (input)
        debounce_state_ = DebounceStates::kDown1;
      else
        debounce_state_ = DebounceStates::kUpIdle;
      break;
  }
}

void Key::UpdateTapState() {
  switch (tap_state_) {
    case TapStates::kIdle:
      if (Pressed())
        tap_state_ = TapStates::kWait;
      break;

    case TapStates::kWait:
      if (tap_cycle_ <= kTapThreshold) {
        if (Released())
          tap_state_ = TapStates::kTap;
        else
          tap_cycle_++;
      } else {
        tap_cycle_ = 0;
        tap_state_ = TapStates::kIdle;
      }
      break;

    case TapStates::kTap:
      tap_cycle_ = 0;
      if (Pressed())
        tap_state_ = TapStates::kWait;
      else
        tap_state_ = TapStates::kIdle;
      break;
  }
}
