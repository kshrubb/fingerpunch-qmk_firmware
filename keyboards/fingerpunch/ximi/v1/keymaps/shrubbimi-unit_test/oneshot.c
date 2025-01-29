#include "oneshot.h"

void register_func(
	uint16_t func
) {
	if (is_modifier_key(func)) {
		register_code(func);
	} else {
		layer_on(func);
	}
}

void unregister_func(
	uint16_t func
) {
	if (is_modifier_key(func)) {
		unregister_code(func);
	} else {
		layer_off(func);
	}
}

void update_oneshot(
    oneshot_state *state,
    uint16_t tap,
	uint16_t hold,
	uint16_t lock,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {	
    if (keycode == trigger) {						// If OS_ key
		if (record->event.pressed) {					// On key down
			switch (*state) {
				case os_up_unqueued:						// If unqueued, register OS_ and set held state
					if (hold != KC_NO) {
						register_func(hold);
						*state = os_down_queued;
					} else {
						register_func(tap);
						*state = os_up_queued;
					}
					break;
				case os_up_queued:							// If queued, set lock state
					if (lock == KC_NO) {
						unregister_func(tap);
						*state = os_up_unqueued;
					} else if (tap != lock) {
						unregister_func(tap);
						register_func(lock);
						*state = os_locked;
					}
					break;
				case os_locked:								// If locked, set unlock state
					unregister_func(lock);
					*state = os_up_unqueued;
					break;
				default:
					break;
			} 
        } else {										// Key up
            switch (*state) {
				case os_down_queued:						// If not used while held, set queue state
					if (tap != hold) {
						unregister_func(hold);
						register_func(tap);
					}
					*state = os_up_queued;
					break;
				case os_down_used:							// If used while held, unregister and reset state
					unregister_func(hold);
					*state = os_up_unqueued;
					break;
				default:
					break;
            }
        }
    } else {										// If not OS_ key
		bool hrm = (is_oneshot_hrm_key(keycode) && tap == read_source_layers_cache(record->event.key));
		if (record->event.pressed) {					// Key down
			if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
				switch (*state) {							// Cancel oneshot on designated cancel keydown
					case os_up_queued:
					case os_up_used:
						unregister_func(tap);
						break;
					case os_down_queued:
					case os_down_used:
						unregister_code(hold);
						break;
					case os_locked:
						unregister_func(lock);
						break;
					default:
						break;
				}
				*state = os_up_unqueued;
			}
			if (!is_oneshot_ignored_key(keycode)) {
				switch (*state) {							// Process key
					case os_up_queued:						// If up and queued, set used state
						*state = os_up_used;
						break;
					case os_down_queued:					// If down and queued, set used state
						*state = os_down_used;
						break;
					case os_up_used:						// If queue is used, unregister mod and reset state
						unregister_func(tap);
						*state = os_up_unqueued;
						break;
					default:
						break;
				}
			}
			if (hrm) {										// Process HRMs separately
				switch (*state) {
					case os_up_queued:
						*state = os_up_used;
						break;
					case os_down_queued:
						*state = os_down_used;
						break;
					default:
						break;
				}
			}
		} else {										// Key up
			if (!is_oneshot_ignored_key(keycode) || (hrm)) {
				switch (*state) {
					case os_up_used:						// If queue is used, unregister mod and reset state
						unregister_func(tap);				// Note: Same functionality occurs on L60. Both are necessary for clean modifier unregistering
						*state = os_up_unqueued;				// due to different cases where mods needs to be cancelled on keyup or keydown
						break;
					default:
						break;
				}
			}
		}
    }
}