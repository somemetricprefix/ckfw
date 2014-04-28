
#line 1 "./tapkeyaction.cpp.rl"
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


#line 83 "./tapkeyaction.cpp.rl"



#line 27 ".\\tapkeyaction.cpp"
static const char _tapkey_actions[] = {
	0, 1, 2, 1, 3, 1, 4, 1, 
	5, 1, 8, 1, 9, 2, 6, 1, 
	2, 8, 2, 3, 0, 2, 1, 3, 
	7, 2, 1
};

static const char _tapkey_key_offsets[] = {
	0, 0, 4, 8, 12
};

static const char _tapkey_trans_keys[] = {
	111, 112, 114, 116, 111, 112, 114, 116, 
	111, 112, 114, 116, 111, 112, 114, 116, 
	0
};

static const char _tapkey_single_lengths[] = {
	0, 4, 4, 4, 4
};

static const char _tapkey_range_lengths[] = {
	0, 0, 0, 0, 0
};

static const char _tapkey_index_offsets[] = {
	0, 0, 5, 10, 15
};

static const char _tapkey_trans_targs[] = {
	1, 2, 1, 1, 0, 3, 2, 4, 
	3, 0, 3, 3, 1, 3, 0, 1, 
	2, 4, 1, 0, 0
};

static const char _tapkey_trans_actions[] = {
	0, 13, 7, 3, 0, 16, 5, 19, 
	9, 0, 0, 5, 11, 3, 0, 1, 
	23, 7, 0, 0, 0
};

static const int tapkey_start = 1;
static const int tapkey_first_final = 5;
static const int tapkey_error = 0;

static const int tapkey_en_main = 1;


#line 86 "./tapkeyaction.cpp.rl"

TapKeyAction::TapKeyAction(u8 row, u8 col, u8 tap_keycode, u8 hold_keycode)
    : row_(row),
      column_(col),
      tap_keycode_(tap_keycode),
      hold_keycode_(hold_keycode),
      timer_{kTapThreshold, row, col} {
  
#line 81 ".\\tapkeyaction.cpp"
	{
	 this->cs = tapkey_start;
	}

#line 94 "./tapkeyaction.cpp.rl"
}

void TapKeyAction::Execute(const Event *ev) {
  char event_character = GetEventCharacter(ev);

  LOG_DEBUG("%c", event_character);

  const char *p = &event_character;
  const char *pe = p + 1;

  
#line 94 ".\\tapkeyaction.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if (  this->cs == 0 )
		goto _out;
_resume:
	_keys = _tapkey_trans_keys + _tapkey_key_offsets[ this->cs];
	_trans = _tapkey_index_offsets[ this->cs];

	_klen = _tapkey_single_lengths[ this->cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _tapkey_range_lengths[ this->cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	 this->cs = _tapkey_trans_targs[_trans];

	if ( _tapkey_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tapkey_actions + _tapkey_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 28 "./tapkeyaction.cpp.rl"
	{
    ReportAddKeycode(tap_keycode_);
    ReportSend();
    ReportRemoveKeycode(tap_keycode_);
  }
	break;
	case 1:
#line 34 "./tapkeyaction.cpp.rl"
	{ TimerStart(&timer_); }
	break;
	case 2:
#line 36 "./tapkeyaction.cpp.rl"
	{ TimerStop(&timer_); }
	break;
	case 3:
#line 38 "./tapkeyaction.cpp.rl"
	{
    LOG_ERROR("Unexpected timeout received. (%2u,%2u)", row_, column_);
  }
	break;
	case 4:
#line 42 "./tapkeyaction.cpp.rl"
	{
    LOG_ERROR("Key press event during down state. (%2u,%2u)", row_, column_);
  }
	break;
	case 5:
#line 46 "./tapkeyaction.cpp.rl"
	{
    LOG_ERROR("Key release event during up state. (%2u,%2u)", row_, column_);
  }
	break;
	case 6:
#line 50 "./tapkeyaction.cpp.rl"
	{ LOG_DEBUG("SetHold"); current_hold_keycode_ = hold_keycode_; }
	break;
	case 7:
#line 52 "./tapkeyaction.cpp.rl"
	{ LOG_DEBUG("SetTap"); current_hold_keycode_ = tap_keycode_; }
	break;
	case 8:
#line 54 "./tapkeyaction.cpp.rl"
	{ ReportAddKeycode(current_hold_keycode_); }
	break;
	case 9:
#line 56 "./tapkeyaction.cpp.rl"
	{ ReportRemoveKeycode(current_hold_keycode_); }
	break;
#line 206 ".\\tapkeyaction.cpp"
		}
	}

_again:
	if (  this->cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 105 "./tapkeyaction.cpp.rl"
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
