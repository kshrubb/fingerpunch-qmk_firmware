#pragma once

#include QMK_KEYBOARD_H

// Represents the six states a oneshot key can be in
typedef enum {
    os_up_unqueued,
    os_up_queued,
	os_up_used,
    os_down_queued,
    os_down_used,
	os_locked,
} oneshot_state;

// Custom dual role oneshot mod implementation that doesn't rely on timers. If
// a mod is	used while it is held it will be unregistered on keyup as normal, 
// otherwise it will be queued and only released after the next non-mod keyup.
void update_oneshot(
    oneshot_state *state,
    uint16_t tap,
	uint16_t hold,
	uint16_t lock,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

void register_func(uint16_t func);

void unregister_func(uint16_t func);

// To be implemented by the consumer. Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// To be implemented by the consumer. Defines keys to ignore when determining
// whether a oneshot mod has been used. Setting this to modifiers and layer
// change keys allows stacking multiple oneshot modifiers, and carrying them
// between layers.
bool is_oneshot_ignored_key(uint16_t keycode);

bool is_oneshot_hrm_key(uint16_t keycode);

bool is_modifier_key(uint16_t keycode);