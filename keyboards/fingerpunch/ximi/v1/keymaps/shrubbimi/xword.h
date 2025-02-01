#pragma once

#include QMK_KEYBOARD_H

typedef enum {
    xw_off,
	xw_on,
	xw_used,
} xword_state;

void set_sym_word_layer(uint8_t layer);

void enable_caps_word(void);
void disable_caps_word(void);

void enable_sym_word(void);
void disable_sym_word(void);

bool is_xword_enabled(xword_state *state);
bool is_caps_word_enabled(void);
bool is_sym_word_enabled(void);

bool update_caps_word(
	uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

bool update_sym_word(
	uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

void caps_word_set_user(xword_state *state, bool active);
bool caps_word_press_user(xword_state *state, uint16_t keycode);

void sym_word_set_user(xword_state *state, bool active);
bool sym_word_press_user(xword_state *state, uint16_t keycode);