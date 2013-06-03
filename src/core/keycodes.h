// Most code in this file is taken from t.m.k Keyboard Keyboard Project
// (https://github.com/tmk/tmk_keyboard) and is therefore licensed under GPL.

#ifndef CKFW_SRC_KEYCODES_H_
#define CKFW_SRC_KEYCODES_H_

/*
 * Short names for ease of definition of keymap
 */
#define KC_LCTL KC_LCTRL
#define KC_RCTL KC_RCTRL
#define KC_LSFT KC_LSHIFT
#define KC_RSFT KC_RSHIFT
#define KC_ESC  KC_ESCAPE
#define KC_BSPC KC_BSPACE
#define KC_ENT  KC_ENTER
#define KC_DEL  KC_DELETE
#define KC_INS  KC_INSERT
#define KC_CAPS KC_CAPSLOCK
#define KC_RGHT KC_RIGHT
#define KC_PGDN KC_PGDOWN
#define KC_PSCR KC_PSCREEN
#define KC_SLCK KC_SCKLOCK
#define KC_PAUS KC_PAUSE
#define KC_BRK  KC_PAUSE
#define KC_NLCK KC_NUMLOCK
#define KC_SPC  KC_SPACE
#define KC_MINS KC_MINUS
#define KC_EQL  KC_EQUAL
#define KC_GRV  KC_GRAVE
#define KC_RBRC KC_RBRACKET
#define KC_LBRC KC_LBRACKET
#define KC_COMM KC_COMMA
#define KC_BSLS KC_BSLASH
#define KC_SLSH KC_SLASH
#define KC_SCLN KC_SCOLON
#define KC_QUOT KC_QUOTE
#define KC_APP  KC_APPLICATION
#define KC_NUHS KC_NONUS_HASH
#define KC_NUBS KC_NONUS_BSLASH
#define KC_ERAS KC_ALT_ERASE,
#define KC_CLR  KC_CLEAR
/* Japanese specific */
#define KC_ZKHK KC_GRAVE
#define KC_RO   KC_INT1
#define KC_KANA KC_INT2
#define KC_JYEN KC_INT3
#define KC_HENK KC_INT4
#define KC_MHEN KC_INT5
/* Keypad */
#define KC_P1   KC_KP_1
#define KC_P2   KC_KP_2
#define KC_P3   KC_KP_3
#define KC_P4   KC_KP_4
#define KC_P5   KC_KP_5
#define KC_P6   KC_KP_6
#define KC_P7   KC_KP_7
#define KC_P8   KC_KP_8
#define KC_P9   KC_KP_9
#define KC_P0   KC_KP_0
#define KC_PDOT KC_KP_DOT
#define KC_PCMM KC_KP_COMMA
#define KC_PSLS KC_KP_SLASH
#define KC_PAST KC_KP_ASTERISK
#define KC_PMNS KC_KP_MINUS
#define KC_PPLS KC_KP_PLUS
#define KC_PEQL KC_KP_EQUAL
#define KC_PENT KC_KP_ENTER

enum kKeyCodes {
  KC_NO               = 0x00,
  KC_ROLL_OVER,
  KC_POST_FAIL,
  KC_UNDEFINED,
  KC_A,
  KC_B,
  KC_C,
  KC_D,
  KC_E,
  KC_F,
  KC_G,
  KC_H,
  KC_I,
  KC_J,
  KC_K,
  KC_L,
  KC_M,               /* 0x10 */
  KC_N,
  KC_O,
  KC_P,
  KC_Q,
  KC_R,
  KC_S,
  KC_T,
  KC_U,
  KC_V,
  KC_W,
  KC_X,
  KC_Y,
  KC_Z,
  KC_1,
  KC_2,
  KC_3,               /* 0x20 */
  KC_4,
  KC_5,
  KC_6,
  KC_7,
  KC_8,
  KC_9,
  KC_0,
  KC_ENTER,
  KC_ESCAPE,
  KC_BSPACE,
  KC_TAB,
  KC_SPACE,
  KC_MINUS,
  KC_EQUAL,
  KC_LBRACKET,
  KC_RBRACKET,        /* 0x30 */
  KC_BSLASH,          /* \ (and |) */
  KC_NONUS_HASH,      /* Non-US # and ~ */
  KC_SCOLON,          /* ; (and :) */
  KC_QUOTE,           /* ' and " */
  KC_GRAVE,           /* Grave accent and tilde */
  KC_COMMA,           /* , and < */
  KC_DOT,             /* . and > */
  KC_SLASH,           /* / and ? */
  KC_CAPSLOCK,
  KC_F1,
  KC_F2,
  KC_F3,
  KC_F4,
  KC_F5,
  KC_F6,
  KC_F7,              /* 0x40 */
  KC_F8,
  KC_F9,
  KC_F10,
  KC_F11,
  KC_F12,
  KC_PSCREEN,
  KC_SCKLOCK,
  KC_PAUSE,
  KC_INSERT,
  KC_HOME,
  KC_PGUP,
  KC_DELETE,
  KC_END,
  KC_PGDOWN,
  KC_RIGHT,
  KC_LEFT,            /* 0x50 */
  KC_DOWN,
  KC_UP,
  KC_NUMLOCK,
  KC_KP_SLASH,
  KC_KP_ASTERISK,
  KC_KP_MINUS,
  KC_KP_PLUS,
  KC_KP_ENTER,
  KC_KP_1,
  KC_KP_2,
  KC_KP_3,
  KC_KP_4,
  KC_KP_5,
  KC_KP_6,
  KC_KP_7,
  KC_KP_8,            /* 0x60 */
  KC_KP_9,
  KC_KP_0,
  KC_KP_DOT,
  KC_NONUS_BSLASH,    /* Non-US \ and | */
  KC_APPLICATION,
  KC_POWER,
  KC_KP_EQUAL,
  KC_F13,
  KC_F14,
  KC_F15,
  KC_F16,
  KC_F17,
  KC_F18,
  KC_F19,
  KC_F20,
  KC_F21,             /* 0x70 */
  KC_F22,
  KC_F23,
  KC_F24,
  KC_EXECUTE,
  KC_HELP,
  KC_MENU,
  KC_SELECT,
  KC_STOP,
  KC_AGAIN,
  KC_UNDO,
  KC_CUT,
  KC_COPY,
  KC_PASTE,
  KC_FIND,
  KC__MUTE,
  KC__VOLUP,          /* 0x80 */
  KC__VOLDOWN,
  KC_LOCKING_CAPS,    /* locking Caps Lock */
  KC_LOCKING_NUM,     /* locking Num Lock */
  KC_LOCKING_SCROLL,  /* locking Scroll Lock */
  KC_KP_COMMA,
  KC_KP_EQUAL_AS400,  /* equal sign on AS/400 */
  KC_INT1,
  KC_INT2,
  KC_INT3,
  KC_INT4,
  KC_INT5,
  KC_INT6,
  KC_INT7,
  KC_INT8,
  KC_INT9,
  KC_LANG1,           /* 0x90 */
  KC_LANG2,
  KC_LANG3,
  KC_LANG4,
  KC_LANG5,
  KC_LANG6,
  KC_LANG7,
  KC_LANG8,
  KC_LANG9,
  KC_ALT_ERASE,
  KC_SYSREQ,
  KC_CANCEL,
  KC_CLEAR,
  KC_PRIOR,
  KC_RETURN,
  KC_SEPARATOR,
  KC_OUT,             /* 0xA0 */
  KC_OPER,
  KC_CLEAR_AGAIN,
  KC_CRSEL,
  KC_EXSEL,           /* 0xA4 */

  /* Modifiers */
  KC_LCTRL            = 0xE0,
  KC_LSHIFT,
  KC_LALT,
  KC_LGUI,
  KC_RCTRL,
  KC_RSHIFT,
  KC_RALT,
  KC_RGUI,
};

#endif // CKFW_SRC_KEYCODES_H_
