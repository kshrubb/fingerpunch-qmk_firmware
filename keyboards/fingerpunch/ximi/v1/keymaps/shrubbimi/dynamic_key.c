#include "dynamic_key.h"

static uint16_t dk_timeout = 150;
static uint16_t dk_timer = 0;
static bool dk_state = true;

void set_dynamic_key_timeout_interval(uint16_t interval) {
	dk_timeout = interval;
}

bool is_dynamic_state_enabled(void) {
	return dk_state;
}

bool update_dynamic_state(uint16_t keycode, keyrecord_t *record, uint16_t trigger, uint16_t repeat_key, uint16_t alt_repeat_key) {
	if (record->event.pressed) {
		if (timer_elapsed(dk_timer) > dk_timeout) {
			dk_state = false;
		}
		
		dk_timer = timer_read();
		
		uint16_t alt_key = get_dynamic_keycode(keycode);
		if (keycode != trigger) {
			if (dk_state) {  // If dynamic state enabled, handle keypress
				if (alt_key != KC_NO) {  // If alt_key defined KC_NO, user handles keypress
					if (alt_key == repeat_key) {
						trigger_repeat_key();
						return false;
					} else if (alt_key == alt_repeat_key) {
						trigger_alt_repeat_key();
						return false;
					} else if (alt_key == trigger) {
						keycode = trigger;
					} else {
						tap_code(alt_key);
					}
				} else {
					return true;
				}
			}
		}
		dk_state = (keycode != trigger); // Update state based on keycode
	}
	return true;
}

__attribute__((weak)) uint16_t get_dynamic_keycode(uint16_t keycode) {
	return KC_NO;
}

__attribute__((weak)) bool is_dynamic_timer_ignored_key(uint16_t keycode) {
	return KC_NO;
}