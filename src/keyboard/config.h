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

// Helper macro for easy port definions.
// p - Letter of the port registers. Expands to PORTB, DDRB and PINB.
// b - nth bit the port register.
#define PORT(p, b, ENTRY) ENTRY(PORT##p, DDR##p, PIN##p, b, kPort##p##b)

// Pin mapping for row pins.
#define MATRIX_ROW_PORTS(ENTRY) \
  PORT(B, 1, ENTRY) \
  PORT(B, 0, ENTRY) \
  PORT(B, 3, ENTRY) \
  PORT(B, 2, ENTRY)

// Pin mapping for column pins.
#define MATRIX_COLUMN_PORTS(ENTRY) \
  PORT(F, 0, ENTRY) \
  PORT(F, 1, ENTRY) \
  PORT(F, 5, ENTRY) \
  PORT(F, 4, ENTRY) \
  PORT(F, 6, ENTRY) \
  PORT(F, 7, ENTRY) \
  PORT(B, 6, ENTRY) \
  PORT(B, 5, ENTRY) \
  PORT(B, 4, ENTRY) \
  PORT(D, 7, ENTRY)

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
