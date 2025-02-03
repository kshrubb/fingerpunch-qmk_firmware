#include "xword.h"

static xword_state xw_cw_state = xw_off;
static xword_state xw_sw_state = xw_off;

static uint8_t sym_layer;

void set_sym_word_layer(uint8_t layer) {
	sym_layer = layer;
}

void enable_caps_word(void) {
	if (xw_cw_state != xw_off) { return; }
	#ifdef XWORD_NO_EDIT
		xw_cw_state = xw_used;
	#else
		xw_cw_state = xw_on;
	#endif
	caps_word_set_user(&xw_cw_state, true);
}

void disable_caps_word(void) {
	if (xw_cw_state == xw_off) { return; }
	xw_cw_state = xw_off;
	caps_word_set_user(&xw_cw_state, false);
}

void enable_sym_word(void) {
	if (xw_sw_state != xw_off) { return; }
	if (IS_LAYER_OFF(sym_layer)) {
		layer_on(sym_layer);
	}
	#ifdef XWORD_NO_EDIT
		xw_sw_state = xw_used;
	#else
		xw_sw_state = xw_on;
	#endif
	sym_word_set_user(&xw_sw_state, true);
}

void disable_sym_word(void) {
	if (xw_sw_state == xw_off) { return; }
	if (IS_LAYER_ON(sym_layer)) {
		layer_off(sym_layer);
	}
	layer_off(sym_layer);
	xw_sw_state = xw_off;
	sym_word_set_user(&xw_sw_state, false);
}

bool is_caps_word_enabled(void) {
	return is_xword_enabled(&xw_cw_state);
}

bool is_sym_word_enabled(void) {
	return is_xword_enabled(&xw_sw_state);
}

bool is_xword_enabled(xword_state *state) {
	switch (*state) {
		case xw_on:
		case xw_used:
			return true;
		default:
			return false;
	}
	return false;
}

bool update_caps_word(uint16_t trigger, uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {		// caps word
		if (keycode == trigger) {
			if (xw_cw_state == xw_off) {
				enable_caps_word();
			} else {
				disable_caps_word();
			}
			return false;
		} else if (xw_cw_state != xw_off) {
			if (caps_word_press_user(&xw_cw_state, keycode)) {
				return true;
			} else {
				return false;
			}
		}
	}
	return true;
}

bool update_sym_word(uint16_t trigger, uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {		// Sym Word
		if (keycode == trigger) {
			if (xw_sw_state == xw_off) {
				enable_sym_word();
			} else {
				disable_sym_word();
			}
			return false;
		} else if (xw_sw_state != xw_off) {
			if (sym_word_press_user(&xw_sw_state, keycode)) {
				return true;
			} else {
				return false;
			}
		}
	}
	return true;
}

__attribute__((weak)) void caps_word_set_user(xword_state *state, bool active) {}
__attribute__((weak)) void sym_word_set_user(xword_state *state, bool active) {}

__attribute__((weak)) bool caps_word_press_user(xword_state *state, uint16_t keycode) {
	
	switch (keycode) {	
		
		// Remove this block if you are using KC_LCAP in caps_word_set_user()
		case KC_A ... KC_Z:
			if (!host_keyboard_led_state().caps_lock) {
				add_weak_mods(MOD_BIT(KC_LSFT));
				return true;  // Keycodes that continue Caps Word, with shift applied
			}
			
		case KC_HOME ... KC_UP:
			return true;  // Keycodes that are ignored by Caps Word
		
		case KC_SPC:
		case KC_TAB:
		case KC_ENT:
			if (*state == xw_used) {  
				disable_caps_word();
			}
			return true;  // Keycodes that disable Caps Word, if a non-ignored key has been pressed
		
		default:
			if (*state == xw_on) {
				*state = xw_used;
			}
			return true;  // Keycodes that continue Caps Word, and aren't ignored
	}
	return false;
}

__attribute__((weak)) bool sym_word_press_user(xword_state *state, uint16_t keycode) {
	
	switch (keycode) {
				
		case KC_HOME ... KC_UP:
			return true;  // Keycodes that are ignored by Sym Word
		
		case KC_SPC:
		case KC_TAB:
		case KC_ENT:
			if (*state == xw_used) {
				disable_sym_word();
			}
			return true;  // Keycodes that disable Sym Word, if a non-ignored key has been pressed
		
		default:
			if (*state == xw_on) {
				*state = xw_used;
			}
			return true;  // Keycodes that continue Sym Word, and aren't ignored
	}
	return false;
}