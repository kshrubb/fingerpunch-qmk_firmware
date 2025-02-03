#pragma once

#include QMK_KEYBOARD_H

void set_dynamic_key_timeout_interval(uint16_t interval);

bool is_dynamic_state_enabled(void);

bool update_dynamic_state(uint16_t keycode, keyrecord_t *record, uint16_t trigger, uint16_t repeat_key, uint16_t alt_repeat_key);

void trigger_repeat_key(void);
void trigger_alt_repeat_key(void);

uint16_t get_dynamic_keycode(uint16_t keycode);