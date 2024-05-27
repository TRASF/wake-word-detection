/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "command_responder.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"

// Ensure you have the display initialized somewhere in your setup
extern "C" void setup_display();

// TODO 1: Create styles for different background colors ---------------------

// END TODO 1 ----------------------------------------------------------------

void setup_styles() {
    // TODO 2 : Initialize styles --------------------------------------------

    // END TODO 2 ------------------------------------------------------------
}

void RespondToCommand(int32_t current_time, const char* found_command,
                      float score, bool is_new_command) {
    if (is_new_command) {
        MicroPrintf("Heard %s (%.4f) @%dms", found_command, score, current_time);

        // Display the recognized command on the LCD
        static lv_obj_t *label = nullptr;
        static lv_obj_t *screen = nullptr;
        if (label == nullptr) {
            screen = lv_scr_act();
            label = lv_label_create(screen);

            // Set label size to avoid overflow
            lv_obj_set_width(label, LV_HOR_RES - 20);
            lv_obj_set_height(label, LV_VER_RES / 4); // Set a sufficient height for the label

            // Enable word wrap and auto resize
            lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
            lv_label_set_recolor(label, true);

            // Set text style to increase size
            static lv_style_t style;
            lv_style_init(&style);

            // TODO 3: Set font size --------------------------------------------

            // END TODO 3 -------------------------------------------------------

            lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);
            lv_obj_add_style(label, &style, 0);
        } else {
            lv_label_set_text(label, ""); // Clear previous text
        }

        // Convert the score to a string
        char score_str[32];
        snprintf(score_str, sizeof(score_str), "%.2f", score);

        // TODO 4 : Create the final string to display -------------------------

        // END TODO 4 ----------------------------------------------------------

        // Set the text of the label
        lv_label_set_text(label, display_str);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // Center the label

        // TODO 5: Remove previous background style -----------------------------

        // END TODO 5 -----------------------------------------------------------

        // TODO 6: Change background color based on the prediction result -------

        // END TODO 6 -----------------------------------------------------------

        // Force display update
        lv_refr_now(NULL);
    }
}
