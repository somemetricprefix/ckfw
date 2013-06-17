#ifndef CKFW_SRC_CONFIG_H_
#define CKFW_SRC_CONFIG_H_

#include <avr/io.h>

// Enable Teensy onboard LED.
#define ERROR() \
  DDRD |= (1 << 6); \
  PORTD |= (1 << 6); \
  for (;;);

// Defines the number of rows in the keyboard matrix.
#define MATRIX_NUM_ROWS 4

// Defines the number of columns in the keyboard matrix.
#define MATRIX_NUM_COLUMNS 10

// Pin mapping for row pins.
#define MATRIX_ROW_PORTS(PORT) \
  PORT(B, 0) \
  PORT(B, 1) \
  PORT(B, 2) \
  PORT(B, 3)

// Pin mapping for column pins.
#define MATRIX_COLUMN_PORTS(PORT) \
  PORT(F, 0) \
  PORT(F, 1) \
  PORT(F, 4) \
  PORT(F, 5) \
  PORT(F, 6) \
  PORT(F, 7) \
  PORT(B, 6) \
  PORT(B, 5) \
  PORT(B, 4) \
  PORT(D, 7)

// BuTECK layout
// B U . , X P C L M F
// H I E A O D T R N S
// K Y Ö Ä Ü J G W V Z
#define KEYMAP \
{ KC_B, KC_U, KC_DOT, KC_COMM, KC_X, KC_P, KC_C, KC_L, KC_M, KC_F }, \
{ KC_H, KC_I, KC_E, 0, KC_O, KC_D, 0, KC_R, KC_N, KC_S }, \
{ KC_K, KC_Z, KC_SCLN, KC_QUOT, KC_LBRC, KC_J, KC_G, KC_W, KC_V, KC_Y }, \
{ 0, 0, KC_TAB, KC_SPC, KC_ENT, 0, KC_BSPACE, 0, 0, 0 }

#endif // CKFW_SRC_CONFIG_H_
