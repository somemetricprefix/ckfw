#include "key.h"

// Debounce routine is implemented with a state machine that models an three
// step integrator. This function was originally based on code written by
// Kenneth a. Kuhn that can be found at:
// http://www.kennethkuhn.com/electronics/debounce.c
void Key::Debounce(bool input) {
  switch (debounce_state) {
    case DebounceStates::kUpIdle:
      if (input)
        debounce_state = DebounceStates::kDown1;
      break;

    case DebounceStates::kDown1:
      if (input)
        debounce_state = DebounceStates::kDown2;
      else
        debounce_state = DebounceStates::kUpIdle;
      break;

    case DebounceStates::kDown2:
      if (input) {
        down = true;
        debounce_state = DebounceStates::kPressed;
      } else {
        debounce_state = DebounceStates::kDown1;
      }
      break;

    case DebounceStates::kPressed:
      if (input)
        debounce_state = DebounceStates::kDownIdle;
      else
        debounce_state = DebounceStates::kUp1;
      break;

    case DebounceStates::kDownIdle:
      if (!input)
        debounce_state = DebounceStates::kUp1;
      break;

    case DebounceStates::kUp1:
      if (input)
        debounce_state = DebounceStates::kDownIdle;
      else
        debounce_state = DebounceStates::kUp2;
      break;

    case DebounceStates::kUp2:
      if (input) {
        debounce_state = DebounceStates::kUp1;
      } else {
        down = false;
        debounce_state = DebounceStates::kReleased;
      }
      break;

    case DebounceStates::kReleased:
      if (input)
        debounce_state = DebounceStates::kDown1;
      else
        debounce_state = DebounceStates::kUpIdle;
      break;
  }
}
