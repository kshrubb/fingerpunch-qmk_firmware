#include QMK_KEYBOARD_H

#include "oneshot.h"

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
    BASE = SAFE_RANGE,
	
	RESET,

	OS_SHFT,
	OS_SCAPS,
    OS_CTRL,
    OS_ALT,
    OS_GUI,

	OS_CNAV,
	OS_SYM,
	OS_FN,
};

#define ______ KC_TRNS
#define XXXXXX KC_NO

#define _BASE 0
#define _ALT  1
#define _SYM  3
#define _NAV  4
#define _FN   5

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

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
	CAPWRD,		KC_A,		KC_S,		KC_D,		KC_F,		KC_G,		KC_H,		KC_J,		KC_K,		KC_L,		KC_QUOT,	SYMWRD,
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
	______,		SL_LINE,	KC_HOME,	KC_UP,		KC_END,		KC_PGUP,	XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		______,
	______,		SL_WORD,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	KC_PGDN,	XXXXXX,		OS_CTRL,	OS_SHFT,	OS_ALT,		OS_GUI,		______,
	______,		C(KC_Z),	C(KC_X),	C(KC_C),	C(KC_V),	C(KC_Y),	XXXXXX,		PAREN,		BRACK,		BRACE,		ARROW,		______,
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
				______,					______,		______,		______,		______,		______,		______,					______,
										XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX,		XXXXXX		// thumb switch
)
};

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;
oneshot_state os_scaps_state = os_up_unqueued;
oneshot_state os_cnav_state = os_up_unqueued;
oneshot_state os_sym_state = os_up_unqueued;
oneshot_state os_fn_state = os_up_unqueued;

// OneShot user-defined keys
bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
		case RESET:
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

bool is_modifier_key(uint16_t keycode) {
	switch (keycode) {
		case KC_LSFT:
		case KC_LCTL:
		case KC_LALT:
		case KC_LGUI:
		case KC_RSFT:
		case KC_RCTL:
		case KC_RALT:
		case KC_RGUI:
		case KC_LCAP:
			return true;
		default:
			return false;
	}
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	
	// update_oneshot(
	//	&os_xxxx_state		Tap			Hold		Lock		Trigger
	//	keycode, record
	// );
	update_oneshot(
        &os_shft_state,		KC_LSFT,	KC_LSFT,	KC_LSFT,	OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state,		KC_LCTL,	KC_LCTL,	KC_LCTL, 	OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state,		KC_LALT,	KC_LALT,	KC_LALT, 	OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state,		KC_LGUI,	KC_LGUI,	KC_LGUI, 	OS_GUI,
        keycode, record
    );
	update_oneshot(
        &os_scaps_state,	KC_LSFT,	KC_LSFT,	KC_LCAP,	OS_SCAPS,
        keycode, record
    );
	update_oneshot(
        &os_cnav_state,		KC_LCTL,	_NAV,		_NAV,		OS_CNAV,
        keycode, record
    );
	update_oneshot(
        &os_sym_state,		_SYM,		_SYM,		_SYM,		OS_SYM,
        keycode, record
    );
	update_oneshot(
        &os_fn_state, 		_FN,		_FN,		_FN,		OS_FN,
        keycode, record
    );
}