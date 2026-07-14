/* Copyright 2020 Josef Adamcik
 * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#define OS_LANG_DELAY 20
#define H_A LGUI_T(KC_A)
#define H_S LALT_T(KC_S)
#define H_D LSFT_T(KC_D)
#define H_F LCTL_T(KC_F)

#define H_J RCTL_T(KC_J)
#define H_K RSFT_T(KC_K)
#define H_L LALT_T(KC_L)
#define H_SCLN RGUI_T(KC_SCLN)

#define M_ENT  MT(MOD_LSFT, KC_ENT)
#define M_SPC  MT(MOD_RSFT, KC_SPC)
#define L_BSPC LT(HIGH, KC_BSPC)
#define L_CH_LANG  LT(LOW, CH_SYS_LANG)


enum souffle_layers {
    MAIN = 0,
    _NUMPAD,
    USUAL,
    LOW,
    HIGH
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAIN] = { ENCODER_CCW_CW(KC_PGDN, KC_PGUP),           ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [LOW] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [HIGH] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)},
    [USUAL] = { ENCODER_CCW_CW(KC_PGDN, KC_PGUP),           ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NUMPAD] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)}
};
#endif



void leader_end_user(void) {
        if (leader_sequence_two_keys(KC_F, KC_Q)) {
            SEND_STRING("https://zoom.us/j/6998722138?pwd=fDbSFi4Oug8ueTesOwuUPyxWRh1FlJ.1");
        }
        if (leader_sequence_two_keys(KC_F, KC_T)) {
            SEND_STRING("https://telemost.yandex.ru/j/29080231100141");
        } 
        if (leader_sequence_two_keys(KC_F, KC_J)) {
            SEND_STRING("https://salutejazz.ru/calls/ilqcsu?psw=OEUaC0tUCRRdVR0dFEAeB1JfAA");
        }                   
}

int kb_language;
int kb_layout;


enum kb_lang {
    ENGLISH,
    RUSSIAN
};



#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };

    oled_write_P(qmk_logo, false);
}

static void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n"), false);

    switch (get_highest_layer(default_layer_state)) {
        case 0: // _QWERTY
            oled_write_P(PSTR("Qwrt\n"), false);
            break;
        default:
            oled_write_P(PSTR("Mod\n"), false);
            break;
    }
    oled_write_P(PSTR("\n"), false);
    // Print current layer
    switch (kb_layout) {
        case 0: // _QWERTY
            oled_write_P(PSTR("W+Spc\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("A+Sft\n"), false);
            break;
    }
    switch (get_highest_layer(layer_state)) {
        case MAIN: // _QWERTY
            oled_write_P(PSTR("Base"), false);
            break;
        case LOW:
            oled_write_P(PSTR("Lower"), false);
            break;
        case HIGH:
            oled_write_P(PSTR("Raise"), false);
            break;    
        case USUAL:
            oled_write_P(PSTR("Usual"), false);
            break;   
        case _NUMPAD:
            oled_write_P(PSTR("Numpad"), false);
            break; 
        default:
            oled_write_P(PSTR("Undef"), false);
            break; 

    }
    oled_write_P(PSTR("\n\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("CPSLK"), led_usb_state.caps_lock);

    if (kb_language == RUSSIAN) {
        oled_write_P(PSTR("Rus"), false);
    } else if (kb_language == ENGLISH){
        oled_write_P(PSTR("Eng"), false);
    }
    oled_write_P(PSTR("\n\n\n"), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }
    return false;
}

#endif


enum custome_keycodes {
    CH_KB_LANG = SAFE_RANGE,
    CH_SYS_LANG,
    CH_LAYOUT,
    _DQUO,
    _QUOT,
    _EXLM,
    _QUES,
    CS_00,
    _COLN,
    _SCLN,
    _ASTR,
    _CIRC,
    _PERC,
    _PIPE,
    _SLSH,
    _BSLS,
    _HASH,
    _AMPR,
    _DLR,
    _AT,
    _LPRN,
    _RPRN,
    _LCBR,
    _RCBR,
    _LT,
    _GT,
    _LBRC,
    _RBRC,
    LS_AT,
    LS_HASH,
    LS_LCBR,
    LS_RCBR,
    LS_GT,
    LS_LT,
    LS_LBRC,
    LS_RBRC,
    LS_LPRN,
    LS_RPRN
};

enum {
    TD_HZ
};



const uint16_t PROGMEM change_lang[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM copy[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM paste[] = {KC_C, KC_V, COMBO_END};
combo_t key_combos[] = {
    COMBO(copy, LCTL(KC_C)),
    COMBO(paste, LCTL(KC_V)),
    COMBO(change_lang, L_CH_LANG),   
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  [   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   CS_00  |TD(TD_HZ)|/|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |LShift | LAlt | LCTR |LOWER | /Enter /       \Space \  |RAISE | KC_F13 | RAlt | RShift |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'            '------''---------------------------'
 */

[MAIN] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_RBRC,
  KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T    ,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,
  KC_TAB,   KC_A ,  KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
  KC_GRV,   KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,     KC_MUTE,   KC_N,    KC_M,    KC_COMM, CS_00,   TD(TD_HZ),KC_RBRC,
  KC_LALT,          KC_LALT, KC_LCTL, L_CH_LANG, M_ENT,                      M_SPC,   L_BSPC,  KC_LCTL,  KC_LGUI,           KC_RSFT
),
[USUAL] = LAYOUT(
  TG(USUAL), KC_1,   KC_2,    KC_3,    KC_4,   KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_RBRC,
  KC_ESC,    KC_Q,   KC_W,    KC_E,    KC_R,   KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_LBRC,
  KC_TAB,    KC_A,   KC_S,    KC_D,    KC_F,   KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
  KC_GRV,    KC_Z,   KC_X,    KC_C,    KC_V,   KC_B, KC_MUTE,   KC_MUTE, KC_N,    KC_M,    KC_COMM, CS_00,   TD(TD_HZ),KC_RBRC,
  KC_LSFT,           KC_LALT, KC_LCTL, L_CH_LANG,KC_ENT,                    M_SPC,   L_BSPC,  KC_LCTL,  XXXXXXX,           KC_RSFT
),
/* NUMPAD
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | trans|      |      |      |      |      |                    |      |NumLck|      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |      |      |      |      |      |                    |   ^  |   7  |   8  |   9  |   *  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | trans|      |      |      |      |      |-------.    ,-------|   -  |   4  |   5  |   6  |      |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | trans|      |      |      |      |      |-------|    |-------|   +  |   1  |   2  |   3  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | Bspc | WIN  |LOWER | Enter| /Space  /       \Enter \  |SPACE | 0    |  .   | RAlt |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_NUMPAD] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  TG(_NUMPAD), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,                   _______, KC_NUM,  XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, QK_RBT, EE_CLR, KC_SLEP,                     KC_PPLS, KC_P7,  KC_P8,   KC_P9,   KC_PAST, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_PMNS, KC_P4,  KC_P5,   KC_P6,   KC_PSLS,  XXXXXXX,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(USUAL),TG(_NUMPAD),   _______,KC_P0, KC_P1,  KC_P2,   KC_P3,   KC_EQL, XXXXXXX,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
              _______, XXXXXXX, _______, _______, _______,   TO(MAIN), L_BSPC, KC_P0 ,   KC_PDOT, KC_EQL
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|    |-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[LOW] = LAYOUT(
  CH_KB_LANG,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  CH_LAYOUT,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_PAST, KC_F12,
  KC_CAPS,   LS_LCBR,   LS_RCBR,   LS_LBRC,   LS_RBRC,   _PIPE,                          KC_PPLS, KC_4,    KC_5,    KC_6,    KC_PSLS,  KC_PSLS,
  _______,   _AMPR, _PERC,   _BSLS,  LS_LT,      LS_GT, TG(_NUMPAD) ,            TG(USUAL),KC_PMNS, KC_1,    KC_2,    KC_3,    KC_EQL, _______,
                      _______, _______, _______, _______, _______,       _______, KC_0, KC_PDOT, _______, _______
),
/* RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |KC_CAPS|CH_LANG|                    |      | PWrd |  Up  | NWrd | DLine| KC_RBRC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | _DQUO| _QUOT |_QUES |_EXLM| _COLN |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |   |       |------+------+------+------+------+------|
 * |Shift | _SCLN |  Cut | Copy | Paste|QK_LEAD|--------|   |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[HIGH] = LAYOUT(
  _______, _______ , _______ , _______ , _______ , _______,          _LCBR,  _RCBR  , _LT,  _GT ,  S(KC_MINS) ,_PERC,
  CW_TOGG,  _ASTR,  LS_AT,   LS_HASH,  _DLR, _CIRC,                        KC_PGUP, _PIPE,   KC_UP, KC_HOME,KC_END, KC_RBRC,
  _SCLN, _DQUO,  _QUOT,  _QUES,_EXLM  , _COLN,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  KC_EQL, S(KC_EQL),S(KC_MINS), KC_MINS, LS_LPRN, LS_RPRN,  _______,       _______,  QK_LEAD, _SLSH, KC_INS, _______,   XXXXXXX, _______,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* ADJUST
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |KC_CAPS|CH_LANG|                    |      | PWrd |  Up  | NWrd | DLine| KC_RBRC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | _DQUO| _QUOT |_QUES |_EXLM| _COLN |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |   |       |------S+------+------+------+------+------|
 * |Shift | _SCLN |  Cut | Copy | Paste|QK_LEAD|--------|   |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'

[ADJUST] = LAYOUT(
  QK_RBT, XXXXXXX , XXXXXXX , XXXXXXX , EE_CLR , KC_SLEP,                           XXXXXXX,  XXXXXXX  , XXXXXXX,  XXXXXXX ,  XXXXXXX ,XXXXXXX,
  XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,  XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,XXXXXXX  , XXXXXXX,                       XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,       XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),  
 */
};


/* uint16_t get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_LSFT, KC_F):
            return TAPPING_TERM - 100;
        case MT(MOD_RSFT, KC_J):
            return TAPPING_TERM - 100;
        case MT(MOD_LCTL, KC_D):
            return TAPPING_TERM + 200;
        case MT(MOD_RCTL, KC_K):
            return TAPPING_TERM + 200;
        default:
            return TAPPING_TERM;
    }
}; */

void switch_layout(void) {// Если переключение языка в системе ALT+SHIFT
    kb_layout++;
    if (kb_layout>1){
        kb_layout = 0;
    }
}


void os_switch_language(void) {
    if (kb_layout == 0){
    // Если переключение языка в системе WIN+SPACE
        register_code16(KC_LWIN);
        register_code16(KC_SPC);
        unregister_code16(KC_LWIN);
        unregister_code16(KC_SPC); 
        SEND_STRING(SS_DELAY(OS_LANG_DELAY));  
    }
     else if (kb_layout == 1){
    // Если переключение языка в системе ALT+SHIFT
        register_code16(KC_LALT);
        register_code16(KC_LSFT);
        unregister_code16(KC_LALT);
        unregister_code16(KC_LSFT); 
        SEND_STRING(SS_DELAY(OS_LANG_DELAY));  
     }
}


void switch_language(void) {// Если переключение языка в системе ALT+SHIFT

   if (kb_layout == 0){
    // Если переключение языка в системе WIN+SPACE
    register_code(KC_LWIN);
    register_code(KC_SPC);
    unregister_code(KC_LWIN);
    unregister_code(KC_SPC);
   }
   else if (kb_layout == 1){
    // Если переключение языка в системе ALT+SHIFT
      register_code(KC_LALT);
      register_code(KC_LSFT);
      unregister_code(KC_LSFT);
      unregister_code(KC_LALT);
   }

    if (kb_language == RUSSIAN) {
        kb_language = ENGLISH;
        print("English\n");
    } else {
        kb_language = RUSSIAN;
        print("Russian\n");
    }
}

void press_hard_znak(tap_dance_state_t *state, void *user_data) {
    uint8_t mod_state;
    mod_state = get_mods();    
    
    if (kb_language == RUSSIAN){
    switch (state->count) {
        case 1:
            //tap_code16(S(KC_SLSH));
            if ( mod_state & MOD_MASK_SHIFT ){
                tap_code16(S(KC_7));
            }
            else {
                tap_code16(S(KC_SLSH));
            }

            break;
        case 2:
            tap_code(KC_RBRC);
            break;
        case 3:
            tap_code(KC_RBRC);
            break;
    }
    reset_tap_dance(state);
    } 
    else {
            if ( mod_state & MOD_MASK_SHIFT ){
                tap_code16(S(KC_SLSH));
            }
            else {
                tap_code(KC_COMM);
            }
    }
}

tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_HZ] = ACTION_TAP_DANCE_FN(press_hard_znak)
};

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;
        case CS_00:
        case KC_LBRC:
        case KC_QUOT:
        case KC_RBRC:
        case KC_SCLN:
        case KC_COMM: 
            if (kb_language == RUSSIAN){
                add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
                return true;
            }
            else
                return false;
        default:
            return false;  // Deactivate Caps Word.
    }
}





uint8_t mod_state;
uint8_t mods;

static bool _pressed = false;


void sw_lang(enum custome_keycodes KC, keyrecord_t *record){
    if (record->event.pressed){
        if (kb_language == RUSSIAN){
                if (!_pressed) {
                    os_switch_language();
                    _pressed = !_pressed;
                }                 
                register_code16(KC);
            }
        else if (kb_language == ENGLISH){
                register_code16(KC);
        }
    } else {
        if (kb_language == RUSSIAN){ 
                if (_pressed){
                    os_switch_language();           
                    _pressed = !_pressed;
                }               
                unregister_code16(KC);  
            }
        else if (kb_language == ENGLISH){
                unregister_code16(KC);
        }
    }
}



bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  mod_state = get_mods();
  mods = 0;
    switch (keycode) {
        case L_CH_LANG: {
            if (record->event.pressed && record->tap.count) {
                switch_language();
                return false;
            }
            return true;
        }
    case CH_KB_LANG:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                kb_language = ENGLISH;
                print ("English\n");
            }
            else if (kb_language == ENGLISH){
                kb_language = RUSSIAN;
                print ("Russian\n");
            }  
            return false;
        }
    }
    case CH_LAYOUT:{
        if (record->event.pressed){
            switch_layout();
        }
        return false;
    }    
    //Обработка буквы "Ю" в русской раскладке. Если включен русский язык, тогда регистрируем KC_DOT, что соответствует букве Ю в русской раскладке. 
    case KC_RBRC: {
         if (record->event.pressed){
            if (kb_language == RUSSIAN) {
                register_code(KC_DOT);
                return false;
            }
         }
         else {
            if (kb_language == RUSSIAN) {
                unregister_code(KC_DOT);
                return false;
            }
         }
        return true;
    }
    case LT(HIGH, KC_BSPC):{
        static bool delkey_registered;
         if (record->event.pressed && record->tap.count){
            if (mod_state & MOD_MASK_SHIFT ){
                del_mods(MOD_MASK_SHIFT);
                register_code(KC_DEL);
                delkey_registered = true;
                set_mods(mod_state);
                return false;
            }
         }
         else {
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
         }
        return true;
    }   
    case KC_BSPC: {
        static bool delkey_registered;
         if (record->event.pressed){
            print("KC_BSPC\n");
            if (mod_state & MOD_MASK_SHIFT ){
                print("MOD_MASK_SHIFT\n");
                del_mods(MOD_MASK_SHIFT);
                register_code(KC_DEL);
                delkey_registered = true;
                set_mods(mod_state);
                return false;
            }
         }
         else {
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
         }
        return true;
    }   
       
    //Обработка точки.
    case CS_00:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                if ( mod_state & MOD_MASK_SHIFT ){
                    register_code(KC_1);
                }
                else {
                    register_code(KC_SLSH);
                }
            }

            if (kb_language == ENGLISH){
                 if (mod_state & MOD_MASK_SHIFT){
                    register_code(KC_1);
                }
                else {
                    register_code(KC_DOT);
                }
            }
        } else {
                unregister_code(KC_SLSH);
                unregister_code(KC_DOT);
                unregister_code(KC_1);
        }
        return false;
    }       
    case _EXLM:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_1));
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_EXLM);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(S(KC_1));
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_EXLM);
            }
        }
        return false;
    }  
    case _QUES:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_7));
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_QUES);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(S(KC_7));
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_QUES);
            }
        }
        return false;
    }  
    case _DQUO:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_2));
                }
            else if (kb_language == ENGLISH){
                    register_code16(S(KC_QUOT));
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(S(KC_2));
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(S(KC_QUOT));
            }
        }
        return false;
    }   
    case _QUOT:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P3);
                    register_code16(KC_P9);
                    set_mods(mod_state);   
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_QUOT);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P3);
                    unregister_code16(KC_P9);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_QUOT);
            }
        }
        return false;
    }    

    case _SCLN:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_4));
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_SCLN);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(S(KC_4));
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_SCLN);
            }
        }
        return false;
    }    

    case _COLN:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_6));
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_COLN);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(S(KC_6));
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_COLN);
            }
        }
        return false;
    }  

    case _ASTR:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(KC_PAST);
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_ASTR);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_PAST);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_ASTR);
            }
        }
        return false;
    }      
    case _CIRC:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P9);
                    register_code16(KC_P4);
                    set_mods(mod_state);   
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_CIRC);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P9);
                    unregister_code16(KC_P4);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_CIRC);
            }
        }
        return false;
    }     
    case _PERC:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_5));
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_PERC);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(S(KC_5));
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_PERC);
            }
        }
        return false;
    } 
    case _PIPE:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P1);
                    register_code16(KC_P2);
                    register_code16(KC_P4);
                    set_mods(mod_state);   
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_PIPE);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P1);
                    unregister_code16(KC_P2);
                    unregister_code16(KC_P4);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_PIPE);
            }
        }
        return false;
    }    
    case _SLSH:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(KC_PSLS);
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_SLSH);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_PSLS);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_SLSH);
            }
        }
        return false;
    }
    case _BSLS:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(KC_BSLS);
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_BSLS);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_BSLS);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_BSLS);
            }
        }
        return false;
    }    
    case _AMPR:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P3);
                    register_code16(KC_P8);
                    set_mods(mod_state);
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_AMPR);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P3);
                    unregister_code16(KC_P8);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_AMPR);
            }
        }
        return false;
    }

    case _DLR:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P3);
                    register_code16(KC_P6);
                    set_mods(mod_state);
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_DLR);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P3);
                    unregister_code16(KC_P6);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_DLR);
            }
        }
        return false;
    }

    case LS_AT:{
        sw_lang(KC_AT, record);
        return false;
    }  
    case LS_HASH:{
        sw_lang(KC_HASH, record);
        return false;
    }  
    case LS_LCBR:{
        sw_lang(KC_LCBR, record);
        return false;
    }  
    case LS_RCBR:{
        sw_lang(KC_RCBR, record);
        return false;
    }  
    case LS_LT:{
        sw_lang(KC_LT, record);
        return false;
    }  
    case LS_GT:{
        sw_lang(KC_GT, record);
        return false;
    } 
    case LS_LBRC:{
        sw_lang(KC_LBRC, record);
        return false;
    }   
    case LS_RBRC:{
        sw_lang(KC_RBRC, record);
        return false;
    } 
    case LS_LPRN:{
        sw_lang(KC_LPRN, record);
        return false;
    }   
    case LS_RPRN:{
        sw_lang(KC_RPRN, record);
        return false;
    } 
}


    return true;
}
