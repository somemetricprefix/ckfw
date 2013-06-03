#ifndef CKFW_SRC_CONFIG_H_
#define CKFW_SRC_CONFIG_H_

// Defines the number of rows in the keyboard matrix.
#define MATRIX_NUM_ROWS 4

// Defines the number of columns in the keyboard matrix.
#define MATRIX_NUM_COLUMNS 10

// Pin mapping for row pins.
#define MATRIX_ROW_PORTS \
  IO_PORT(B, 1), \
  IO_PORT(B, 0), \
  IO_PORT(B, 3), \
  IO_PORT(B, 2),

// Pin mapping for column pins.
#define MATRIX_COLUMN_PORTS \
  IO_PORT(F, 0), \
  IO_PORT(F, 1), \
  IO_PORT(F, 5), \
  IO_PORT(F, 4), \
  IO_PORT(F, 6), \
  IO_PORT(F, 7), \
  IO_PORT(B, 6), \
  IO_PORT(B, 5), \
  IO_PORT(B, 4), \
  IO_PORT(D, 7),

// BuTECK layout
// B U . , X P C L M F
// H I E A O D T R N S
// K Y Ö Ä Ü J G W V Z
#define KEYMAP \
{ KC_B, KC_U, KC_DOT, KC_COMM, KC_X, KC_P, KC_C, KC_L, KC_M, KC_F }, \
{ KC_H, KC_I, KC_E, KC_A, KC_O, KC_D, KC_T, KC_R, KC_N, KC_S }, \
{ KC_K, KC_Z, KC_SCLN, KC_QUOT, KC_LBRC, KC_J, KC_G, KC_W, KC_V, KC_Y }, \
{ 0, 0, KC_TAB, KC_SPC, KC_ENT, 0, 0, 0, 0, 0 }

#endif // CKFW_SRC_CONFIG_H_
