#include <zephyr/kernel.h>
#include "util.h"
#include <ctype.h>
#include <stdio.h>


void to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void fill_background(lv_obj_t *canvas) {
    lv_draw_rect_dsc_t rect_black_dsc;
    rect_black_dsc.bg_color = lv_color_white();  // set background color to white
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);

    lv_canvas_draw_rect(canvas, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, &rect_black_dsc);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font = font;
    label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}

// ZMK only reports a percentage (the peripheral never sends its voltage), computed in
// zmk/app/module/drivers/sensor/battery/battery_common.c as:
// pct = mV * 2 / 15 - 459, clamped to 3450..4200 mV.
// Invert that to recover an approximate voltage (to within ~7.5 mV).
void battery_level_to_voltage_str(uint8_t level, char *buf, size_t len) {
    if (level == 0) {
        snprintf(buf, len, "--");
        return;
    }
    int mv = (level + 459) * 15 / 2;
    if (level >= 100) {
        mv = 4200;
    }
    snprintf(buf, len, "%d.%02dV", mv / 1000, (mv % 1000) / 10);
}
