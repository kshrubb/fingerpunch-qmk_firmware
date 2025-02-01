#pragma once

#include QMK_KEYBOARD_H

// Represents the six states a oneshot key can be in
typedef enum {
    os_up_unqueued,		// Keyup 	-> no state queued
    os_down_queued,		// Keydown -> held state queued for use
    os_down_used,		// Keydown	-> held state used, tap state will not queue
    os_up_queued,		// Keyup	-> tap state queued
	os_up_used,			// Keyup	-> tap state used, state returns to unqueued state on any following key-down or key-up
	os_locked,			// Down x2	-> lock state triggered, returns to unqueued state on next press of OS_xxxx key
} oneshot_state;

// Custom Tri-Role One-Shot Mod/Layer implementation that doesn't
// rely on timers. If a mod or layer is used while it is held it
// will be unregistered on keyup as normal, otherwise it will be
// queued and released after the next non-mod keyup. 
// Tap, hold and lock states can be configured individually 
// per key to be any mod or layer. Hold and lock states can be 
// disabled by setting them to KC_NO. If the lock state is disabled,
// the second keypress will cancel the first tap. Check the keymap
// for examples on how to configure these keys.
void update_oneshot(
    oneshot_state *state,
    uint16_t tap,
	uint16_t hold,
	uint16_t lock,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

// Helper functions to distinguish and handle layers vs modifiers
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