#include QMK_KEYBOARD_H
#include "quantum.h"
#ifdef OLED_ENABLE
#include "vnoiram_oled.h"
#endif

#include "vnoiram.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

enum click_state {
    WAITING = 0, // waiting with no type and move mice and scroll (mouse layer is off)
    TYPING, // typing some keys that not mouse keys (mouse layer is off)
    CLICKABLE, // can click with mouse layer is on (mouse layeer is on)
    // CLICKING, // click with mouse click key (mouse layer is on)
    SCROLLING // scrolling (mouse layer is on)
};

enum click_state c_state;
enum click_state old_c_state;
uint16_t click_timer;
uint16_t type_timer;
report_mouse_t old_report;
