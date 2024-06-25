/*
This is the c configuration file for the keymap

Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#endif

#ifdef AUTO_SHIFT_ENABLE
#    define AUTO_SHIFT_TIMEOUT 105
#    define NO_AUTO_SHIFT_ALPHA
#    define NO_AUTO_SHIFT_NUMERIC
#endif

#ifdef TAP_DANCE_ENABLE
#    define TAPPING_TERM 100
#    define TAPPING_TERM_PER_KEY
// #    define RETRO_TAPPING_PER_KEY
// #    define IGNORE_MOD_TAP_INTERRUPT
#endif

#ifdef LEADER_ENABLE
#    define LEADER_TIMEOUT 400
#    define LEADER_PER_KEY_TIMING
#    define LEADER_NO_TIMEOUT
#endif

#if defined(ALT_TAB_BY_LAYER_ENABLE) || defined(ALT_TAB_ENABLE)
#  define ALT_TAB_TIMEOUT 85
#endif

# define CLICKMODE_PREVENT_MOVEMENT 125

// keyball retrcit layer to 8 bit
// expand 8bit -> 16 bit
// #    undef LAYER_STATE_8BIT
// #    define LAYER_STATE_16BIT
