#include QMK_KEYBOARD_H
#include "quantum.h"
#ifdef OLED_ENABLE
#include "vnoiram_oled.h"
#endif

#include "vnoiram.h"
#include "keymap.h"

#ifdef CONSOLE_ENABLE
#include "print.h"

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  // debug_matrix=true;:
  // debug_keyboard=true;
  // debug_mouse=true;
}
#endif

void disable_cmode(void){
  layer_off(_DEFAULT_MICE);
  c_state = WAITING;
}

int16_t my_abs(int16_t num) {
    if (num < 0) {
        num = -num;
    }
    return num;
}

#if defined(LAYER_STATE_8BIT)
// for spcl and rnum
void proc_spcl_rnum(keyrecord_t *record, uint16_t regist_keycode){
  if (record->event.pressed) {
    register_code(KC_LGUI);
    tap_code16(regist_keycode);
    unregister_code(KC_LGUI);
  }
}
#endif

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
#if defined(LAYER_STATE_8BIT)
  if (layer_state_is(_SPCL) && layer_state_is(_RNUM)) {
    switch (keycode) {
      case KC_6:
        proc_spcl_rnum(record, KC_LEFT);
        return false;
      case KC_7:
        proc_spcl_rnum(record, KC_DOWN);
        return false;
      case KC_8:
        proc_spcl_rnum(record, KC_UP);
        return false;
      case KC_9:
        proc_spcl_rnum(record, KC_RGHT);
        return false;
    }
  }
#endif

  switch (keycode) {
    case SCRL_TO:
#ifdef CONSOLE_ENABLE
      print("scrl-to\n");
#endif
      click_timer = timer_read();
      if (record->event.pressed) {
        if (!keyball_get_scroll_mode()) {
          c_state = SCROLLING;
        } else {
          c_state = CLICKABLE;
        }
      }
      break;
    case KC_MS_BTN1 ... KC_MS_BTN8:
#ifdef CONSOLE_ENABLE
      print("btn\n");
#endif
      click_timer = timer_read();
      c_state = CLICKABLE;
      break;
    case KC_LCTL:
    case KC_LSFT:
    case KC_LALT:
    case KC_LGUI:
      if (c_state == CLICKABLE) {
        break;
      }
    default:
      type_timer = timer_read();
#ifdef CONSOLE_ENABLE
      print("mise: default\n");
#endif
      if (c_state != WAITING && c_state != TYPING) {
        disable_cmode();
      }
      c_state = TYPING;
      break;
  }
  return true;
}

void matrix_scan_keymap(void) {
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  if (has_mouse_report_changed(mouse_report, old_report)) {
    if (c_state == WAITING || (c_state == TYPING && (my_abs(mouse_report.x) + my_abs(mouse_report.y)) > CLICKMODE_PREVENT_MOVEMENT)) {
      c_state = CLICKABLE;
      layer_on(_DEFAULT_MICE);
    }
    click_timer = timer_read();
  } else {
    if (c_state == TYPING){
      if (timer_elapsed(type_timer) > 550) {
        c_state = WAITING;
      }
    } else if (c_state == CLICKABLE) {
      if (timer_elapsed(click_timer) > 300) {
       disable_cmode();
      }
    }
  }
  old_report = mouse_report;
  return mouse_report;
}

// layer declarations
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for development
  [_DEFAULT_KEY] = LAYOUT_universal(
    KC_Q           , KC_W         , KC_E       , KC_R             , KC_T            ,                                           KC_Y             , KC_U    , KC_I    , KC_O   , KC_P             ,
    // LCTL_T(KC_A)   , SCRL_TO      , KC_BTN2    , KC_BTN1          , TO_KEY_TIME     ,                                           SCRL_TO          , KC_BTN1 , KC_BTN2 , TO_KEY_TIME , KC_BTN3          ,
    LCTL_T(KC_A)   , KC_S         , KC_D       , KC_F             , KC_G            ,                                           KC_H             , KC_J    , KC_K    , KC_L   , KC_SCLN          ,
    LSFT_T(KC_Z)   , LGUI_T(KC_X) , LALT_T(KC_C)       , KC_V             , KC_B            ,                                           KC_N             , KC_M    , KC_COMM , KC_DOT , KC_SLSH          ,
    LCTL_T(KC_ESC) , KC_LGUI      , MY_ALT_TAB , LT(_SPCL,KC_ESC) , LT(_SFT,KC_SPC) , LT(_SCRL,KC_TAB) , LT(_ALTMOVE,KC_BSPC) , LT(_RNUM,KC_ENT) , KC_NO   , KC_NO   , KC_NO  , TO(_DEFAULT_MICE)
  ),
  
  [_DEFAULT_MICE] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                          _______ , _______ , _______ , _______ , _______           ,
    // _______ , KC_S    , KC_D    , KC_F    , KC_G    ,                          KC_H    , KC_J    , KC_K    , KC_L    , KC_SCLN           ,
    // _______ , SCRL_TO , KC_BTN2 , KC_BTN1 , _______ ,                          SCRL_TO , KC_BTN1 , KC_BTN2 , _______ , KC_BTN3           ,
    _______ , SCRL_TO , KC_BTN2 , KC_BTN1 , KC_BTN3 ,                          SCRL_TO , KC_BTN1 , KC_BTN2 , _______ , _______           ,
    _______ , _______ , _______ , _______ , _______ ,                          _______ , _______ , _______ , _______ , _______           ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , TO(_DEFAULT_KEY)
  ),

#ifdef ONE_HAND_GAME_ENABLE
  [_ONE_HAND_GAME] = LAYOUT_universal(
    KC_Q           , KC_E    , KC_W       , KC_R             , KC_T   ,                                KC_Y             , KC_U  , KC_I    , KC_O   , KC_P              ,
    KC_LSFT        , KC_A    , KC_S       , KC_D             , KC_G   ,                                KC_H             , KC_J  , KC_K    , KC_L   , KC_SCLN           ,
    KC_LCTL        , KC_Z    , KC_X       , KC_V             , KC_B   ,                                KC_N             , KC_M  , KC_COMM , KC_DOT , KC_SLSH           ,
    KC_C , KC_LGUI , KC_ESC , KC_BTN3 , KC_SPC , LT(_RNUM, KC_F) , LT(_ALTMOVE,KC_BSPC) , LT(_RNUM,KC_ENT) , KC_NO , KC_NO   , KC_NO  , TO(_DEFAULT_MICE)
  ),
#endif

  [_SFT] = LAYOUT_universal(
    S(KC_Q) , S(KC_W) , S(KC_E) , S(KC_R) , S(KC_T) ,                          S(KC_Y) , S(KC_U) , S(KC_I) , S(KC_O) , S(KC_P)    ,
    S(KC_A) , S(KC_S) , S(KC_D) , S(KC_F) , S(KC_G) ,                          S(KC_H) , S(KC_J) , S(KC_K) , S(KC_L) , S(KC_SCLN) ,
    S(KC_Z) , S(KC_X) , S(KC_C) , S(KC_V) , S(KC_B) ,                          S(KC_N) , S(KC_M) , KC_LBRC , KC_RBRC , KC_BSLS    ,
    _______ , _______ , _______ , _______ , _______ , _______ ,       KC_DEL , _______ , _______ , _______ , _______ , _______
  ),

  [_SPCL] = LAYOUT_universal(
    KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5   ,                          KC_F6   , KC_F7   , KC_F8   , KC_F9   , KC_F10     ,
    // _______ , KC_MPLY , KC_VOLU , KC_MINS , KC_QUOT ,                          KC_PGUP , KC_BTN1 , KC_BTN2 , KC_BTN3 , KC_EQL     ,
    _______ , SCRL_TO , KC_BTN2 , KC_BTN1 , KC_BTN3 ,                          KC_PGUP , KC_MINS , KC_QUOT , KC_GRV  , KC_EQL     ,
    _______ , KC_MPLY , KC_MUTE , KC_VOLD , KC_VOLU ,                          KC_PGDN , KC_HOME , KC_END  , _______ , _______    ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , JTU_TOGGLE
  ),

  [_SCRL] = LAYOUT_universal(
    KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5    ,                          KC_F6        , KC_F7    , KC_F8    , KC_F9   , KC_F10   ,
    _______ , _______ , _______ , KC_F11  , SCRL_DVI ,                          KC_LEFT      , KC_DOWN  , KC_UP    , KC_RGHT , KC_LEAD  ,
    _______ , _______ , _______ , KC_F12  , SCRL_DVD ,                          CPI_D1K      , CPI_D100 , CPI_I100 , CPI_I1K , KBC_SAVE ,
    _______ , KBC_RST , _______ , _______ , _______  , _______ ,      _______ , LALT(KC_ENT) , _______  , _______  , KBC_RST , _______
  ),

  [_ALTMOVE] = LAYOUT_universal(
    KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5   ,                          KC_F6         , KC_F7         , KC_F8       , KC_F9         , KC_F10  ,
    KC_LCTL , _______ , _______ , KC_F11  , KC_QUOT ,                          LALT(KC_LEFT) , LALT(KC_DOWN) , LALT(KC_UP) , LALT(KC_RGHT) , KC_QUOT ,
    _______ , _______ , _______ , KC_F12  , KC_MINS ,                          KC_GRV        , KC_EQL        , KC_LBRC     , KC_RBRC       , KC_BSLS ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______       , _______       , _______     , _______       , _______
  ),

  [_RNUM] = LAYOUT_universal(
    S(KC_1) , S(KC_2) , S(KC_3) , S(KC_4) , S(KC_5) ,                          S(KC_6) , S(KC_7) , S(KC_8) , S(KC_9) , S(KC_0) ,
    KC_1    , KC_2    , KC_3    , KC_4    , KC_5    ,                          KC_6    , KC_7    , KC_8    , KC_9    , KC_0    ,
    _______ , KC_MPLY , KC_MUTE , KC_VOLU , KC_VOLD ,                          KC_HOME , KC_END  , KC_LBRC , KC_RBRC , KC_BSLS ,
#ifdef ONE_HAND_GAME_ENABLE
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , TO(_ONE_HAND_GAME)
#else
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , _______
#endif
  ),

#ifdef ALT_TAB_BY_LAYER_ENABLE
  [_ALTTAB] = LAYOUT_universal(
    _______ , _______ , _______    , _______ , _______ ,                          _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______    , _______ , _______ ,                          KC_H    , KC_J    , KC_K    , KC_L    , _______ ,
    _______ , _______ , _______    , _______ , _______ ,                          _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , MY_ALT_TAB , _______ , _______ , _______ ,      _______ , KC_ENT  , _______ , _______ , _______ , _______
  ),
#endif

#if !defined(LAYER_STATE_8BIT) || defined(LAYER_STATE_16BIT) || defined(LAYER_STATE_32BIT)
  [_SPCL_AND_RNUM] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                          _______       , _______       , _______     , _______       , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                          LWIN(KC_LEFT) , LWIN(KC_DOWN) , LWIN(KC_UP) , LWIN(KC_RGHT) , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                          _______       , _______       , _______     , _______       , _______ ,
    _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______       , _______       , _______     , _______       , _______
  ),
#endif

};

// clang-format on
// keyball scroll layer
layer_state_t layer_state_set_user(layer_state_t state) {
  bool f = (get_highest_layer(state) == _SCRL || get_highest_layer(state) == _SFT);
  // Auto enable scroll mode when the highest layer is 4
  keyball_set_scroll_mode(f, (get_highest_layer(state) == _SFT));
  if (f) {
    if (c_state != SCROLLING) {
      old_c_state = c_state;
    }
    c_state = SCROLLING;
  } else {
    if (c_state != SCROLLING) {
      old_c_state = c_state;
    }
  }
#ifdef CONSOLE_ENABLE
  // print("in layer state: ");
  // switch (get_highest_layer(state)) {
  //   case _DEFAULT_MICE:
	// 		print("DEFAULT_MICE");
  //     break;
  //   case _DEFAULT_KEY:
	// 		print("DEFAULT_KEY");
  //     break;
  //   case _SFT:
	// 		print("SFT");
  //     break;
  //   case _SPCL:
	// 		print("SPCL");
  //     break;
  //   case _SCRL:
	// 		print("SCRL");
  //     break;
  //   case _RNUM:
	// 		print("RNUM");
  //     break;
  //   case _ALTMOVE:
	// 		print("ALTMOVE");
  //     break;
  //   case _ALTTAB:
	// 		print("ALTTAB");
  //     break;
  //   case _SPCL_AND_RNUM:
  //     print("_SPCL_AND_RNUM");
  //     break;
  //   default:
	// 		print("Undefined");
  // }
#endif 
#if !defined(LAYER_STATE_8BIT) || defined(LAYER_STATE_16BIT) || defined(LAYER_STATE_32BIT)
  state = update_tri_layer_state(state, _SPCL, _RNUM, _SPCL_AND_RNUM);
#endif 
  // state = update_tri_layer_state(state, _RAISE, _SYMB, _SPECIAL);
  return state;
}
