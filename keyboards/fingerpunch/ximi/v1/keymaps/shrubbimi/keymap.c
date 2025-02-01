#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "xword.h"
#include "repeat_key.h"
#include "select_word.h"

// Layer names
enum layer_names {
    _BASE,
	_ALT,
	_SYM,
    _NAV,
	_FN
};

// Custom keycodes
enum custom_keycodes {
    BASE = PLACEHOLDER_SAFE_RANGE,
	ROT0,
	ROT1,
	RESET,
	PANIC,
	LDEF0,
	LDEF1,
	SL_WORD,
	LN_JOIN,
	PAREN,
	BRACK,
	BRACE,
	ARROW,
	DQUOT,
	IDE,
	CAPWRD,
	SYMWRD,
	OS_SHFT,
	OS_SCAPS,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
	OS_CNAV,
	OS_SYM,
	OS_FN,
	REPEAT,
};

// Combos
enum combos {
	QW_PANC,
	WE_ESC,
	SD_DEL,
	DF_BSPC,
	XC_TAB,
	CV_ENT,

	JK_BSPC,
	KL_DEL,
};

const uint16_t PROGMEM qw_combo[] = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM we_combo[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM sd_combo[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM df_combo[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM xc_combo[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM cv_combo[] = {KC_C, KC_V, COMBO_END};

const uint16_t PROGMEM jk_combo[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM kl_combo[] = {KC_K, KC_L, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
	[QW_PANC] = COMBO(qw_combo, PANIC),
	[WE_ESC] = COMBO(we_combo, KC_ESCAPE),
	[SD_DEL] = COMBO(sd_combo, KC_DEL),
	[DF_BSPC] = COMBO(df_combo, KC_BSPC),
	[XC_TAB] = COMBO(xc_combo, KC_TAB),
	[CV_ENT] = COMBO(cv_combo, KC_ENT),

	[JK_BSPC] = COMBO(jk_combo, LCTL(KC_BSPC)),
	[KL_DEL] = COMBO(kl_combo, LCTL(KC_DEL)),
};

// Key Overrides
const key_override_t base_comma_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_COLN);
const key_override_t base_dot_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_SCLN);

const key_override_t **key_overrides = (const key_override_t *[]) {
	&base_comma_override,
	&base_dot_override,
	NULL
};

#define ______ KC_TRNS
#define XXXXXX KC_NO

#define _BASE 0
#define _ALT  1
// #define _ALT2 2
#define _SYM  3
#define _NAV  4
#define _FN   5

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* ----------------------------------------------------------------------------------------------------------------------------------------------------------
||||||||||||||||||||||||||||||||| FEATURES |
--------------------------------------------

	IDE mode - disables macros for ([{<"">}]) and replaces () with ( on tap and ) on shift-tap

	Tap-Dance OSM/OSL
		--> Callum's OSM base https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum
		--> Dalius's OSL addition https://blog.ffff.lt/posts/callum-layers/
		
		### Tri-role Callum style mixed OSMs + OSLs.

		Easily configurable:
		- Customizable tap action, hold action, lock action, and trigger code via simple framework.
		- Lock action and hold action easily disabled per key by setting them to KC_NO. 
			-> Instead of locking, tapping a second time will unregister the OSM/OSL. 

		Functional:
		- Mods and layers can be combined freely for any of the three configurable actions.
		- Mods nor layers seem to get stuck unless you try to OSL into a layer lower than the one you're currently in.
		- Functions properly for layered HRMs both when tapping or holding into a layer. 

		Caveats:
		- Chaining OSLs does not work unless the 2nd OSL is used on an OSM. OSL -> OSL -> OSM
		- Would not recommend 

	Combos:
		WE -> Esc
		SD -> Del
		DF -> BSpc
		XC -> Tab
		CV -> Ent

		JK -> Ctrl + Bspc
		KL -> Ctrl + Del

	
--------------------------------------------
||||||||||||||||||||||||||||||||||||| TODO |
--------------------------------------------

	More NAV tools:
	--> Script keys on NAV?
	--> join lines?
			
	Magic/Leader/Repeat/Alt-Repeat?
	Xcase
		
	Shift+Space key override
	Shift+NumRow overrides for more symbols in sym_word?
	
	Fix Reuer repeat key stickiness?
		upon repeat trigger, flag a condition in the key tracking to stop tracking until the event has been generated
		
	osl code
		Scaps is shift after space or backspace, Repeat key after alphanumeric, 
			reset to shift by default with quick timer after last keypress
		Cnav is magic key or altrep in same fashion as above
		Feed a variable to update_oneshot()
		Fix _FN?
	
	xword code
		shift-reverse for non LCAP users
	
--------------------------------------------
|||||||||||||||||||||||||||||||||||| CHECK |
--------------------------------------------

	Dynamic macro delay
	

---------------------------------------------------------------------------------------------------------------------------------------------------------
||||||||||||||||||||||||||||| USEFUL THINGS |
---------------------------------------------

	

--------------------------------------------------------------------------------------------------------------------------------------------------------- */

/* _BASE
 * ,-----------------------------------------.           			   ,-----------------------------------------.
 * | RESET|   Q  |   W  |   E  |   R  |   T  |           		 	   |   Y  |   U  |   I  |   O  |   P  |  IDE |
 * |------+------+------+------+------+------|          		  	   |------+------+------+------+------+------|
 * |CapWrd|   A  |   S  |   D  |   F  |   G  |          			   |   H  |   J  |   K  |   L  |  ' " |SymWrd|
 * |------+------+------+------+------+------|         				   |------+------+------+------+------+------|
 * |      |   Z  |   X  |   C  |   V  |   B  |         		  		   |   N  |   M  |  , : |  . ; |   /  |      |
 * `-----------------------------------------'         		  		   `-----------------------------------------'
 *        ,------.         		,-----------------------.    ,-----------------------.    		   ,------.
 *        | Rot0 |       		|       |  Ctrl |  Spc  |    |  Bspc |  Shft |       |    		   | Rot1 |
 *        `------'     		    |  _FN  |  _NAV |       |    |       |  Caps |  _SYM |    		   `------'
 *                 				`-----------------------'    `-----------------------' */

[_BASE] = LAYOUT_ximi(
	RESET,		KC_Q,		KC_W,		KC_E,		KC_R,		KC_T,		KC_Y,		KC_U,		KC_I,		KC_O,		KC_P,		IDE,
	CAPWRD,		KC_A,		KC_S,		KC_D,		KC_F,		KC_G,		KC_H,		KC_J,		KC_K,		KC_L,		DQUOT,		SYMWRD,
	EE_CLR,		KC_Z,		KC_X,		KC_C,		KC_V,		KC_B,		KC_N,		KC_M,		KC_COMM,	KC_DOT,		KC_SLSH,	XXXXXX,
				ROT0,					OS_FN,		OS_CNAV,	KC_SPC,		KC_BSPC,	OS_SCAPS,	OS_SYM,					ROT1,
										XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX 		// thumb switch
),
 
/* _ALT
 * ,-----------------------------------------.           			   ,-----------------------------------------.
 * | RESET|   B  |   L  |   D  |   C  |   V  |           		 	   |   J  |   Y  |   O  |   U  |   ,  |  IDE |
 * |------+------+------+------+------+------|          		  	   |------+------+------+------+------+------|
 * |GuiTab|   N  |   R  |   T  |   S  |   G  |          			   |   P  |   H  |   A  |   E  |   I  |      |
 * |------+------+------+------+------+------|         				   |------+------+------+------+------+------|
 * |      |   X  |   Q  |   M  |   W  |   Z  |         		  		   |   K  |   F  |   '  |   ;  |   .  |      |
 * `-----------------------------------------'         		  		   `-----------------------------------------'
 *        ,------.         		,-----------------------.    ,----------------------.    		   ,------.
 *        | Rot0 |       		|       |  Ctrl |  Spc  |    |  Bspc |       |       |    		   | Rot1 |
 *        `------'     		    |  _FN  |  _NAV |       |    |       |  Shft |  _SYM |    		   `------'
 *                 				`-----------------------'    `----------------------' */

[_ALT] = LAYOUT_ximi(
	______,		KC_B,		KC_L,		KC_D,		KC_C,		KC_V,		KC_J,		KC_Y,		KC_O,		KC_U,		KC_COMM,	______,
	______,		KC_N,		KC_R,		KC_T,		KC_S,		KC_G,		KC_P,		KC_H,		KC_A,		KC_E,		KC_I,		______,
	______,		KC_X,		KC_Q,		KC_M,		KC_W,		KC_Z,		KC_K,		KC_F,		KC_QUOT,	KC_SCLN,	KC_DOT,		______,
				______,					______,		______,		______,		______,		______,		______,					______,
										XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX		// thumb switch
),

/* _SYM
 * ,-----------------------------------------.           			   ,-----------------------------------------.
 * | RESET|   !  |   @  |   #  |   $  |   <  |          			   |   >  |   =  |   +  |   -  |   _  |  IDE |
 * |------+------+------+------+------+------|          		  	   |------+------+------+------+------+------|
 * |CapWrd|   1  |   2  |   3  |   4  |   5  |          			   |   6  |   7  |   8  |   9  |   0  |SymWrd|
 * |------+------+------+------+------+------|         				   |------+------+------+------+------+------|
 * |      |   %  |   ^  |   &  |   *  |   `  |         		  		   |   |  |   \  |   ,  |   .  |   /  |      |
 * `-----------------------------------------'         		  		   `-----------------------------------------'
 *        ,------.         		,-----------------------.    ,-----------------------.    		   ,------.
 *        | Rot0 |       		|       |  Ctrl |  Spc  |    |  Bspc |       |       |    		   | Rot1 |
 *        `------'     		    |  _FN  |  _NAV |       |    |       |  Shft |  _SYM |    		   `------'
 *                 				`-----------------------'    `----------------------'  */

[_SYM] = LAYOUT_ximi(
	______,		KC_EXLM,	KC_AT,		KC_HASH,	KC_DLR,		S(KC_COMM),	S(KC_DOT),	KC_EQL,		KC_PLUS,	KC_MINS,	KC_UNDS,	______,
	______,		KC_1,		KC_2,		KC_3,		KC_4,		KC_5,		KC_6,		KC_7,		KC_8,		KC_9,		KC_0,		______,
	______,		KC_PERC,	KC_CIRC,	KC_AMPR,	KC_ASTR,	KC_GRV,		KC_PIPE,	KC_BSLS,	KC_COMM,	KC_DOT,		KC_SLSH,	______,
				______,					______,		______,		______,		______,		______,		______,					______,
										XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX		// thumb switch
),

/* _NAV
 * ,-----------------------------------------.           			   ,-----------------------------------------.
 * | RESET|S_LINE| Home |  Up  |  End | PgUp |          			   |      |      |      |      |      |  IDE |
 * |------+------+------+------+------+------|          		  	   |------+------+------+------+------+------|
 * |CapWrd|S_WORD| Left | Down | Right| PgDn |          			   |      | Ctrl | Shift|  Alt |  GUI |SymWrd|
 * |------+------+------+------+------+------|         				   |------+------+------+------+------+------|
 * |      | Undo |  Cut | Copy | Paste| Redo |        		  		   |      |  ()  |  []  |  {}  |  <>  |      |
 * `-----------------------------------------'         		  		   `-----------------------------------------'
 *        ,------.         		,------------------------.    ,-----------------------.    		   ,------.
 *        | Rot0 |       		|       |  Ctrl |  Spc  |    |  Bspc |       |        |    		   | Rot1 |
 *        `------'     		    |  _FN  |  _NAV |       |    |       |  Shft |  _SYM  |    		   `------'
 *                 				`------------------------'    `-----------------------' */

[_NAV] = LAYOUT_ximi(
	// ______,	XXXXXX,		KC_HOME,	KC_END,		XXXXXX,		LN_JOIN,	XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		______,
	// ______,	KC_LEFT,	KC_DOWN,	KC_UP,		KC_RIGHT,	SL_WORD,	XXXXXX,		OS_CTRL,	OS_SHFT,	OS_ALT,		OS_GUI,		______,
	______,		LN_JOIN,	KC_HOME,	KC_UP,		KC_END,		KC_PGUP,	XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		______,
	______,		SL_WORD,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	KC_PGDN,	XXXXXX,		OS_CTRL,	OS_SHFT,	OS_ALT,		OS_GUI,		______,
	______,		C(KC_Z),	C(KC_X),	C(KC_C),	C(KC_V),	C(KC_Y),	XXXXXX,		PAREN,		BRACK,		BRACE,		ARROW,		______,
				______,					______,		______,		______,		______,		______,		______,					______,
										XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX		// thumb switch
),

/* _FN
 * ,-----------------------------------------.           			   ,-----------------------------------------.
 * | RESET|DMREC1|DMPLY1|DMREC2|DMPLY2|      |          			   |BTLOAD|  F1  |  F2  |  F3  |  F4  |  IDE |
 * |------+------+------+------+------+------|          		  	   |------+------+------+------+------+------|
 * |CapWrd|  GUI |  Alt | Shift| Ctrl |      |           		 	   |EE_CLR|  F5  |  F6  |  F7  |  F8  | _ALT |
 * |------+------+------+------+------+------|          		  	   |------+------+------+------+------+------|
 * |      |StpTrk|PlyTrk|PrvTrk|NxtTrk|      |         		  		   |  IDE |  F9  |  F10 |  F11 |  F12 | _BASE|
 * `-----------------------------------------'         		  		   `-----------------------------------------'
 *        ,------.         		,-----------------------.    ,-----------------------.    		   ,------.
 *        | Rot0 |       		|       |  Ctrl |  Spc  |    |  Bspc |       |       |    		   | Rot1 |
 *        `------'     		    |  _FN  |  _NAV |       |    |       |  Shft |  _SYM |    		   `------'
 *                 				`-----------------------'    `-----------------------' */

[_FN] = LAYOUT_ximi(
	______,		DM_REC1,	DM_PLY1,	DM_REC2,	DM_PLY2,	XXXXXX,		QK_BOOT,	KC_F1,		KC_F2,		KC_F3,		KC_F4,		______,
	______,		OS_GUI,		OS_ALT,		OS_SHFT,	OS_CTRL,	XXXXXX,		EE_CLR,		KC_F5,		KC_F6,		KC_F7,		KC_F8,		LDEF1,
	______,		KC_MSTP,	KC_MPLY,	KC_MPRV,	KC_MNXT,	XXXXXX,		XXXXXX,		KC_F9,		KC_F10,		KC_F11,		KC_F12,		LDEF0,
				______,					OS_FN,		XXXXXX,		______,		______,		______,		XXXXXX,					______,
										XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX		// thumb switch
)
};

// make fingerpunch/ximi/v1:shrubbimi FP_TRACKBALL_BOTH=yes ENCODER_ENABLE=yes AUDIO_ENABLE=no

bool is_ide_active = false;

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

uint16_t saved_mods;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;
oneshot_state os_scaps_state = os_up_unqueued;
oneshot_state os_cnav_state = os_up_unqueued;
oneshot_state os_sym_state = os_up_unqueued;
oneshot_state os_fn_state = os_up_unqueued;

void reset_board(void) {  // Helper function for reset keycodes
	is_ide_active = false;
}

bool caps_word_press_user(xword_state *state, uint16_t keycode) {  // xWord user-defined keys
	switch (keycode) {
			
		case RESET:
		case LDEF0:
		case LDEF1:
			disable_caps_word();
			return true;  // Keycodes that cancel Caps Word
		
		case KC_SPC:
		case KC_TAB:
		case KC_ENT:
			if (*state == xw_used) {
				disable_caps_word();
			}
			return true;  // Keycodes that disable Caps Word, if a non-ignored key has been pressed
		
		case KC_HOME ... KC_UP:
		case OS_SHFT ... OS_FN:
			return true;  // Keycodes that are ignored by Caps Word
		
		default:
			if (*state == xw_on) {
				*state = xw_used;
			}
			return true;  // Keycodes that continue Caps Word, and aren't ignored
	}
	return false;
}

bool sym_word_press_user(xword_state *state, uint16_t keycode) {
	switch (keycode) {
		
		case RESET:
		case LDEF0:
		case LDEF1:
			disable_sym_word();
			return true;  // Keycodes that cancel Sym Word
		
		case KC_SPC:
		case KC_TAB:
		case KC_ENT:
			if (*state == xw_used) {
				disable_sym_word();
			}
			return true;  // Keycodes that disable Sym Word, if a non-ignored key has been pressed
		
		case KC_HOME ... KC_UP:
		case OS_SHFT ... OS_FN:
			return true;  // Keycodes that continue Sym Word, and are ignored
				
		default:
			if (*state == xw_on) {
				*state = xw_used;
			}
			return true;  // Keycodes that continue Sym Word, and aren't ignored
	}
	return false;
}

void caps_word_set_user(xword_state *state, bool active) {
	if (active) {
		if (os_scaps_state == os_locked) {
			os_scaps_state = os_up_unqueued;
		} else {
			register_code(KC_LCAP);
		}
	} else {
		if (os_scaps_state != os_locked) {
			unregister_code(KC_LCAP);
		}
	}
}

void sym_word_set_user(xword_state *state, bool active) {
	if (active) {
		if (os_sym_state != os_up_unqueued) {
			os_sym_state = os_up_unqueued;
		}
		register_code(KC_LNUM);
	} else if (!active) {
		unregister_code(KC_LNUM);
		if (os_sym_state != os_up_unqueued) {
			layer_on(_SYM);
		}
	}
}

bool is_oneshot_cancel_key(uint16_t keycode) {  // OneShot user-defined keys
    switch (keycode) {
		case RESET:
		case LDEF0:
		case LDEF1:
		case CAPWRD:
		case SYMWRD:
			return true;
		default:
			return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
		case OS_SHFT:
		case OS_CTRL:
		case OS_ALT:
		case OS_GUI:
		case OS_SCAPS:
		case OS_CNAV:
		case OS_SYM:
		case OS_FN:
			return true;
		default:
			return false;
    }
}

bool is_oneshot_hrm_key(uint16_t keycode) {
	switch (keycode) {
		case OS_SHFT:
		case OS_CTRL:
		case OS_ALT:
		case OS_GUI:
			return true;
		default:
			return false;
	}
}

void keyboard_post_init_user(void) {
	set_sym_word_layer(_SYM);
}

void matrix_scan_user(void) {
	if (is_alt_tab_active) {		// Rotary encoder alt-tab
		if (timer_elapsed(alt_tab_timer) > 1500) {
			unregister_code(KC_LALT);
			is_alt_tab_active = false;
			set_mods(saved_mods);
		}
	}
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	
	//			   &os_xxxx_state  Tap		Hold	 Lock	  Trigger   keycode  record
	update_oneshot(&os_shft_state, KC_LSFT, KC_LSFT, KC_LSFT, OS_SHFT,  keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, KC_LCTL, KC_LCTL, OS_CTRL,  keycode, record);
    update_oneshot(&os_alt_state,  KC_LALT, KC_LALT, KC_LALT, OS_ALT,   keycode, record);
    update_oneshot(&os_gui_state,  KC_LGUI, KC_LGUI, KC_LGUI, OS_GUI,   keycode, record);
	update_oneshot(&os_scaps_state,KC_LSFT, KC_LSFT, KC_LCAP, OS_SCAPS, keycode, record);
	update_oneshot(&os_cnav_state, KC_LCTL, _NAV,	 _NAV,    OS_CNAV,  keycode, record);
	update_oneshot(&os_sym_state,  _SYM,	_SYM,	 _SYM,    OS_SYM,	keycode, record);
	update_oneshot(&os_fn_state,   _FN,	    _FN,	 _FN,	  OS_FN,	keycode, record);
	
	if (is_caps_word_enabled() && os_scaps_state == os_locked) {
		disable_caps_word();
	} else if (is_sym_word_enabled() && os_sym_state != os_up_unqueued) {
		disable_sym_word();
	}
	
	// 					 Trigger  Keycode  Record
	if (!update_caps_word(CAPWRD, keycode, record)) { return false; }
	if (!update_sym_word( SYMWRD, keycode, record)) { return false; }
	
	switch (keycode) {
		case RESET:  // Reset mods and layers
			if (record->event.pressed) {
				reset_board();
			}
			return false;
		case PANIC:  // Hard reset mods and layers
			if (record->event.pressed) {
				layer_clear();
				clear_mods();
				reset_board();
			}
			return false;
		case LDEF0:  // Set default layer to _BASE and clear layers
			if (record->event.pressed) {
				reset_board();
				set_single_persistent_default_layer(_BASE);
			}
			return false; 
		case LDEF1:  // Set default layer to _ALT and clear layers
			if (record->event.pressed) {
				reset_board();
				set_single_persistent_default_layer(_ALT);
			}
			return false;
		case SL_WORD:  // Select word keycode
			if (record->event.pressed) {
				if (get_mods() == MOD_MASK_CTRL) {  // Ctrl + SL_WORD to select line
					SEND_STRING(SS_TAP(X_END) SS_LSFT(SS_TAP(X_HOME)));
				} else {
					SEND_STRING(SS_LCTL(SS_TAP(X_RIGHT) SS_LSFT(SS_TAP(X_LEFT))));
				}
			}
			return false;
		case LN_JOIN:  // Select line keycode
			if (record->event.pressed) {
				SEND_STRING(SS_TAP(X_END X_DEL X_SPC) SS_LCTL(SS_TAP(X_RIGHT)) SS_LCTL(SS_LSFT(SS_TAP(X_LEFT))) SS_TAP(X_DEL X_SPC));
			}
			return false;
		case PAREN:  // Custom parantheses
			if (record->event.pressed) {
				if (!is_ide_active) {
					SEND_STRING("()" SS_TAP(X_LEFT));
				} else {
					if (get_mods() & MOD_MASK_SHIFT) {
						SEND_STRING(")");
					} else {
						SEND_STRING("(");
					}
				}
			}
			return false;
		case DQUOT:  // Custom brackets
			if (record->event.pressed) {
				if (!is_ide_active) {
					if (get_mods() & MOD_MASK_SHIFT) {
						SEND_STRING("\"\"" SS_TAP(X_LEFT));
					} else {
						tap_code(KC_QUOT);
					}
				} else {
					if (get_mods() & MOD_MASK_SHIFT) {
						SEND_STRING("\"");
					} else {
						tap_code(KC_QUOT);
					}
				}
			}
			return false;
		case BRACK:  // Custom brackets
			if (record->event.pressed) {
				if (!is_ide_active) {
					SEND_STRING("[]" SS_TAP(X_LEFT));
				} else {
					if (get_mods() & MOD_MASK_SHIFT) {
						saved_mods = get_mods();
						clear_mods();
						SEND_STRING("]");
						set_mods(saved_mods);
					} else {
						SEND_STRING("[");
					}
				}
			}
			return false;
		case BRACE:  // Custom braces
			if (record->event.pressed) {
				if (!is_ide_active) {
					SEND_STRING("{}" SS_TAP(X_LEFT));
				} else {
					if (get_mods() & MOD_MASK_SHIFT) {
						SEND_STRING("}");
					} else {
						SEND_STRING("{");
					}
				}
			}
			return false;
		case ARROW:  // Custom arrows
			if (record->event.pressed) {
				if (!is_ide_active) {
					SEND_STRING("<>" SS_TAP(X_LEFT));
				} else {
					if (get_mods() & MOD_MASK_SHIFT) {
						SEND_STRING(">");
					} else {
						SEND_STRING("<");
					}
				}
			}
			return false;
		case IDE:  // Change bracket keys to default/shift-override variant
			if (record->event.pressed) {
				is_ide_active = !is_ide_active;
			}
			return false;
		case ROT0:  // Left encoder keycode
			return false;
		case ROT1:  // Right encoder keycode
			return false;
        default:
			return true;
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {										// Left encoder - Undo/Redo
		if (!clockwise) {
			if (!is_alt_tab_active) {
				is_alt_tab_active = true;
				saved_mods = get_mods();
				clear_mods();
				register_code(KC_LALT);
			}
			alt_tab_timer = timer_read();
			tap_code16(KC_TAB);
		} else {
		  if (!is_alt_tab_active) {
			is_alt_tab_active = true;
			register_code(KC_LALT);
		  }
		  alt_tab_timer = timer_read();
		  tap_code16(S(KC_TAB));
		}
	} else if (index == 1) {								// Right encoder - Volume-/+
		if (clockwise) {
			tap_code(KC_VOLD);
		} else {
			tap_code(KC_VOLU);
		}
	}
	return false;
};

