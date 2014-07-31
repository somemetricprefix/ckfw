
#line 1 "tapkey.rl"
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


#line 75 "tapkey.rl"



#line 27 "tapkey.c"
static const char _tapkey_actions[] = {
	0, 1, 2, 1, 3, 1, 6, 1, 
	7, 2, 0, 1, 2, 4, 1, 2, 
	5, 1
};

static const char _tapkey_key_offsets[] = {
	0, 0, 3, 6, 9
};

static const char _tapkey_trans_keys[] = {
	112, 114, 120, 112, 114, 120, 112, 114, 
	120, 112, 114, 120, 0
};

static const char _tapkey_single_lengths[] = {
	0, 3, 3, 3, 3
};

static const char _tapkey_range_lengths[] = {
	0, 0, 0, 0, 0
};

static const char _tapkey_index_offsets[] = {
	0, 0, 4, 8, 12
};

static const char _tapkey_trans_targs[] = {
	2, 1, 1, 0, 2, 3, 4, 0, 
	2, 3, 1, 0, 4, 1, 4, 0, 
	0
};

static const char _tapkey_trans_actions[] = {
	12, 3, 0, 0, 1, 9, 5, 0, 
	15, 3, 0, 0, 1, 7, 0, 0, 
	0
};

static const int tapkey_start = 1;
static const int tapkey_first_final = 5;
static const int tapkey_error = 0;

static const int tapkey_en_main = 1;


#line 78 "tapkey.rl"

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

  
#line 101 "tapkey.c"
	{
	 tk->cs = tapkey_start;
	}

#line 108 "tapkey.rl"
}

void TapKeyExecute(struct TapKey *tk, Event ev) {
  const bool this_key = (ev.row == tk->row && ev.column == tk->column);
  char event_character = GetEventCharacter(ev, this_key);
  if (event_character == '\0') {
    return;
  }

  LOG_DEBUG("%c", event_character);

  const char *p = &event_character;
  const char *pe = p + 1;

  
#line 118 "tapkey.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if (  tk->cs == 0 )
		goto _out;
_resume:
	_keys = _tapkey_trans_keys + _tapkey_key_offsets[ tk->cs];
	_trans = _tapkey_index_offsets[ tk->cs];

	_klen = _tapkey_single_lengths[ tk->cs];
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

	_klen = _tapkey_range_lengths[ tk->cs];
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
	 tk->cs = _tapkey_trans_targs[_trans];

	if ( _tapkey_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tapkey_actions + _tapkey_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 28 "tapkey.rl"
	{
    ReportAddKeycode(tk->tap_keycode);
    ReportSend();
    ReportRemoveKeycode(tk->tap_keycode);
  }
	break;
	case 1:
#line 34 "tapkey.rl"
	{ TimerStart(&tk->timer); }
	break;
	case 2:
#line 36 "tapkey.rl"
	{
    LOG_ERROR("Key press event during down state. (%2u,%2u)",
              tk->row, tk->column);
  }
	break;
	case 3:
#line 41 "tapkey.rl"
	{
    LOG_ERROR("Key release event during up state. (%2u,%2u)",
              tk->row, tk->column);
  }
	break;
	case 4:
#line 46 "tapkey.rl"
	{ LOG_DEBUG("SetHold"); tk->current_hold_keycode = tk->hold_keycode; }
	break;
	case 5:
#line 48 "tapkey.rl"
	{ LOG_DEBUG("SetTap"); tk->current_hold_keycode = tk->tap_keycode; }
	break;
	case 6:
#line 50 "tapkey.rl"
	{ ReportAddKeycode(tk->current_hold_keycode); }
	break;
	case 7:
#line 52 "tapkey.rl"
	{ ReportRemoveKeycode(tk->current_hold_keycode); }
	break;
#line 224 "tapkey.c"
		}
	}

_again:
	if (  tk->cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 123 "tapkey.rl"
}
