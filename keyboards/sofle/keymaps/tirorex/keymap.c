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

enum souffle_layers {
    MAIN = 0,
    _NUMPAD,
    LOW = 2,
    HIGH,
    ADJUST
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAIN] = { ENCODER_CCW_CW(KC_PGDN, KC_PGUP),           ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [LOW] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(KC_PGDN, KC_PGUP) },
    [HIGH] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)},
    [ADJUST] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)},
    [_NUMPAD] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)}
};
#endif



void leader_end_user(void) {
        if (leader_sequence_two_keys(KC_F, KC_Q)) {
            SEND_STRING("https://zoom.us/j/3267399829?pwd=WEJ5QUNFVDR5YUxCaFFMcFg5eEpIUT09");
        }
        if (leader_sequence_two_keys(KC_F, KC_1)) {
            SEND_STRING("2200151315529490");
        }
        if (leader_sequence_three_keys(KC_F, KC_1, KC_1)) {
            SEND_STRING("06/30/757");
        }
}

int kb_language;


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
    oled_write_P(PSTR("\n\n"), false);

    switch (get_highest_layer(default_layer_state)) {
        case 0: // _QWERTY
            oled_write_ln_P(PSTR("Qwrt\n"), false);
            break;
        default:
            oled_write_P(PSTR("Mod"), false);
            break;
    }
    oled_write_P(PSTR("\n\n"), false);
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case MAIN: // _QWERTY
            oled_write_P(PSTR("Base\n"), false);
            break;
        case LOW:
            oled_write_P(PSTR("Lower"), false);
            break;
        case HIGH:
            oled_write_P(PSTR("Raise"), false);
            break;
        case ADJUST:
            oled_write_P(PSTR("Adjust"), false);
            break;        
        case _NUMPAD:
            oled_write_P(PSTR("Numpad"), false);
            break;   
        default:
            oled_write_ln_P(PSTR("Undef"), false);

    }
    oled_write_P(PSTR("\n\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("CPSLK"), led_usb_state.caps_lock);

    if (kb_language == RUSSIAN) {
        oled_write_P(PSTR("RUSSIAN"), false);
    } else {
        oled_write_P(PSTR("ENGLISH"), false);
    }
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
    CH_LANG = SAFE_RANGE,
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
    _RBRC
};

enum {
    TD_HARD_ZNAK
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
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   CS_00  |TD(TD_HARD_ZNAK)|/|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |LShift | LAlt | LCTR |LOWER | /Enter /       \Space \  |RAISE | KC_F13 | RAlt | RShift |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'            '------''---------------------------'
 */

[MAIN] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    TT(HIGH),                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_RBRC,
  KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_LBRC,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_GRV,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,     KC_MUTE,KC_N,    KC_M, KC_COMM,  CS_00, TD(TD_HARD_ZNAK), KC_RBRC,
                 KC_LSFT,KC_LALT,KC_LCTL, MO(LOW), KC_ENT,      KC_SPC,  LT(HIGH, KC_BSPC), KC_F13, KC_LGUI, KC_RSFT
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
  TG(_NUMPAD), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   _______, KC_NUM,  XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_PPLS, KC_P7,  KC_P8,   KC_P9,   KC_PAST, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_PMNS, KC_P4,  KC_P5,   KC_P6,   KC_PSLS,  XXXXXXX,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,_______,   _______,KC_P0, KC_P1,  KC_P2,   KC_P3,   KC_EQL, XXXXXXX,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
              _______, XXXXXXX, _______, _______, _______,   _______, MO(HIGH), KC_P0 ,   KC_PDOT, KC_EQL
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
  CH_LANG,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12,
  S(KC_EQL), _LCBR,   _RCBR, _LT,  _GT, S(KC_MINS),                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _PIPE,  _BSLS, _LBRC, _RBRC, MO(ADJUST), TG(_NUMPAD), _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                       _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
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
  _______, _______ , _______ , _______ , _______ , _______,          _LCBR,  _RCBR  , _LT,  _GT ,  S(KC_MINS) ,S(KC_EQL),
  _AMPR,  _ASTR,  _AT,   _HASH,  _DLR, _CIRC,                        KC_PGUP, _PIPE,   KC_UP, KC_HOME,KC_END, KC_RBRC,
  _SCLN, _DQUO,  _QUOT,  _QUES,_EXLM  , _COLN,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  KC_EQL, _PERC,S(KC_MINS), KC_MINS, _LPRN, _RPRN,  _______,       _BSLS,  QK_LEAD, _SLSH, KC_INS, _______,   XXXXXXX, _______,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* ADJUST
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
[ADJUST] = LAYOUT(
  QK_RBT, XXXXXXX , XXXXXXX , XXXXXXX , EE_CLR , KC_SLEP,                           XXXXXXX,  XXXXXXX  , XXXXXXX,  XXXXXXX ,  XXXXXXX ,XXXXXXX,
  XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,  XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,XXXXXXX  , XXXXXXX,                       XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,       XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),  

};




void switch_language(void) {
    // Если переключение языка в системе ALT+SHIFT
    /*
      register_code(KC_LALT);
      register_code(KC_LSFT);
      unregister_code(KC_LSFT);
      unregister_code(KC_LALT);
    */

    // Если переключение языка в системе WIN+SPACE
    register_code(KC_LWIN);
    register_code(KC_SPC);
    unregister_code(KC_LWIN);
    unregister_code(KC_SPC);

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
    [TD_HARD_ZNAK] = ACTION_TAP_DANCE_FN(press_hard_znak)
};

uint8_t mod_state;
uint8_t mods;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  mod_state = get_mods();
  mods = 0;
    switch (keycode) {
        case KC_F13: {
            if (record->event.pressed) {
                switch_language();
                return true;
            }
            return true;
        }
    case CH_LANG:{
        if (record->event.pressed){
        if (kb_language == RUSSIAN) {
            kb_language = ENGLISH;
            print ("English\n");
        }


        else {
            kb_language = RUSSIAN;
            print ("Russian\n");
        }
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
    case _HASH:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P3);
                    register_code16(KC_P5);
                    set_mods(mod_state);
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_HASH);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P3);
                    unregister_code16(KC_P5);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_HASH);
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
    case _AT:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    //SEND_STRING(SS_LWIN(" ") SS_DELAY(100) "@"  SS_LWIN(" "));
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P6);
                    register_code16(KC_P4);
                    set_mods(mod_state);  
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_AT);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P6);
                    unregister_code16(KC_P4);                    
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_AT);
            }
        }
        return false;
    }
    case _LCBR:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P1);
                    register_code16(KC_P2);
                    register_code16(KC_P3);
                    set_mods(mod_state);                
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_LCBR);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P1);
                    unregister_code16(KC_P2);
                    unregister_code16(KC_P3);                   
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_LCBR);
            }
        }
        return false;
    }

    case _RCBR:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P1);
                    register_code16(KC_P2);
                    register_code16(KC_P5);
                    set_mods(mod_state);
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_RCBR);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P1);
                    unregister_code16(KC_P2);
                    unregister_code16(KC_P5);                      
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_RCBR);
            }
        }
        return false;
    }

    case _LT:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P6);
                    register_code16(KC_P0);
                    set_mods(mod_state);   
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_LT);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P6);
                    unregister_code16(KC_P0);                    
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_LT);
            }
        }
        return false;
    }

    case _GT:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P6);
                    register_code16(KC_P2);
                    set_mods(mod_state);   
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_GT);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P6);
                    unregister_code16(KC_P2);                       
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_GT);
            }
        }
        return false;
    }

    case _LBRC:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P9);
                    register_code16(KC_P1);
                    set_mods(mod_state);   
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_LBRC);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P9);
                    unregister_code16(KC_P1);                       
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_LBRC);
            }
        }
        return false;
    }

    case _RBRC:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    mods |= MOD_BIT(KC_LALT);
                    set_mods(mods);
                    register_code16(KC_P9);
                    register_code16(KC_P3);
                    set_mods(mod_state);   
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_RBRC);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(KC_P9);
                    unregister_code16(KC_P3);
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_RBRC);
            }
        }
        return false;
    }

    case _LPRN:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_9));
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_LPRN);
            }
        } else {
            if (kb_language == RUSSIAN){
                   unregister_code16(S(KC_9)); 
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_LPRN);
            }
        }
        return false;
    }

    case _RPRN:{
        if (record->event.pressed){
            if (kb_language == RUSSIAN){
                    register_code16(S(KC_0));
                }
            else if (kb_language == ENGLISH){
                    register_code16(KC_RPRN);
            }
        } else {
            if (kb_language == RUSSIAN){
                    unregister_code16(S(KC_0));
                }
            else if (kb_language == ENGLISH){
                    unregister_code16(KC_RPRN);
            }
        }
        return false;
    }



    
}


    return true;
}
