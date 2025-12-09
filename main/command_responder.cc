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
#include <cstdio>
#include <cstring>
#include "bsp/esp32_s3_eye.h"
#include "esp_timer.h"

// Ensure you have the display initialized somewhere in your setup
extern "C" void setup_display();

// Debounce mechanism to prevent excessive display updates
static int64_t last_display_update = 0;
static const int64_t MIN_DISPLAY_UPDATE_INTERVAL_US = 500000; // 500ms minimum between updates
static char last_command[32] = "";

// TODO 1: Create styles for different background colors ---------------------

// END TODO 1 ----------------------------------------------------------------

void setup_styles()
{
    // TODO 2 : Initialize styles --------------------------------------------

    // END TODO 2 ------------------------------------------------------------
}

void RespondToCommand(int32_t current_time, const char *found_command,
                      float score, bool is_new_command)
{
    if (is_new_command)
    {
        MicroPrintf("Heard %s (%.4f) @%dms", found_command, score, current_time);

        // Check if we should update display (debounce)
        int64_t now = esp_timer_get_time();
        bool should_update_display = false;
        
        // Update display if:
        // 1. Enough time has passed since last update, OR
        // 2. Command has changed
        if ((now - last_display_update) > MIN_DISPLAY_UPDATE_INTERVAL_US ||
            strcmp(last_command, found_command) != 0)
        {
            should_update_display = true;
            last_display_update = now;
            strncpy(last_command, found_command, sizeof(last_command) - 1);
            last_command[sizeof(last_command) - 1] = '\0';
        }

        if (!should_update_display)
        {
            // Skip display update to prevent freeze
            return;
        }

        // Display the recognized command on the LCD
        static lv_obj_t *label = nullptr;
        static lv_obj_t *screen = nullptr;
        
        // Try to acquire display lock with timeout to prevent indefinite blocking
        if (!bsp_display_lock(100)) // 100ms timeout
        {
            // Could not acquire lock, skip this update
            return;
        }
        
        if (label == nullptr)
        {
            screen = lv_scr_act();
            label = lv_label_create(screen);

            // Set label size to avoid overflow
            lv_obj_set_width(label, LV_HOR_RES - 20);

            // Enable word wrap and auto resize
            lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

            // Set text style to increase size
            static lv_style_t style;
            lv_style_init(&style);

            // TODO 3: Set font size --------------------------------------------
            lv_style_set_text_font(&style, &lv_font_montserrat_14);
            // END TODO 3 -------------------------------------------------------

            lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);
            lv_obj_add_style(label, &style, 0);
        }

        // TODO 4 : Create the final string to display -------------------------
        char display_str[128];
        snprintf(display_str, sizeof(display_str), "Detected something");
        // END TODO 4 ----------------------------------------------------------

        // Set the text of the label (only if different to minimize updates)
        lv_label_set_text(label, display_str);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); // Center the label

        // TODO 5: Remove previous background style -----------------------------

        // END TODO 5 -----------------------------------------------------------

        // TODO 6: Change background color based on the prediction result -------

        // END TODO 6 -----------------------------------------------------------

        // Unlock display - LVGL will refresh automatically
        bsp_display_unlock();
    }
}
