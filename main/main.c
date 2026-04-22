#include <stdio.h>
#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"

#include "esp_log.h"

#include "lvgl.h"
#include "demos/lv_demos.h"

#include "ui.h"
#include "init_board.h" // 包含新的初始化头文件

#include "coco_detect.h"


#include "screens.h"
#define CAM_W 120
#define CAM_H 88

static volatile int capt_b = 0;
static volatile int face_busy = 0;

lv_color_t *cam_buf;
static lv_img_dsc_t cam_img;
lv_color_t *cam_who_buf ;
static lv_img_dsc_t who_img;
// static lv_color_t cam_cap_buf[CAM_W * CAM_H];
// static lv_img_dsc_t cap_img = {
//     .header.cf = LV_IMG_CF_TRUE_COLOR,
//     .header.w = CAM_W,
//     .header.h = CAM_H,
//     .data_size = CAM_W * CAM_H * 2,
//     .data = (uint8_t *)cam_cap_buf,
// };


void action_camera_capture(lv_event_t * e)
{
    if (!face_busy) {
        capt_b = 1;
    }
}

static inline void rgb565_to_rgb888(
    const uint16_t *src,
    uint8_t *dst,
    int w,
    int h)
{
    int pixels = w * h;

    for (int i = 0; i < pixels; i++) {
        uint16_t p = src[i];

        uint8_t r = (p >> 11) & 0x1F;
        uint8_t g = (p >> 5)  & 0x3F;
        uint8_t b = p & 0x1F;

        // 扩展到8bit（关键）
        dst[i * 3 + 0] = (r << 3) | (r >> 2);
        dst[i * 3 + 1] = (g << 2) | (g >> 4);
        dst[i * 3 + 2] = (b << 3) | (b >> 2);
    }
}

uint8_t *rgb888_buf;

static void face_task(void *param)
{
    face_busy = 1;

    rgb565_to_rgb888(
    (uint16_t*)cam_who_buf,
    rgb888_buf,
    320,
    320
    );
    // AI 推理（耗时）

    coco_detect_run(
        (uint8_t*)rgb888_buf,
        320,
        320,
        (uint8_t*)cam_who_buf
    );

    if (lvgl_lock(-1))
    {
        lv_img_set_src(objects.img_edit, &who_img);
        lvgl_unlock();
    }

    face_busy = 0;
    vTaskDelete(NULL);
}

static void camera_task(void *param)
{
    camera_fb_t *pic;

    cam_buf = heap_caps_malloc(CAM_W * CAM_H * 2, MALLOC_CAP_SPIRAM);

    cam_img.header.cf = LV_IMG_CF_TRUE_COLOR;
    cam_img.header.w = CAM_W;
    cam_img.header.h = CAM_H;
    cam_img.data_size = CAM_W * CAM_H * 2;
    cam_img.data = (uint8_t *)cam_buf;

    cam_who_buf = heap_caps_malloc(320 * 320 * 2, MALLOC_CAP_SPIRAM);

    who_img.header.cf = LV_IMG_CF_TRUE_COLOR;
    who_img.header.w = 320;
    who_img.header.h = 320;
    who_img.data_size = 320 * 320 * 3;
    who_img.data = (uint8_t *)cam_who_buf;

    uint32_t last_ui = 0;

    while (1)
    {
        pic = esp_camera_fb_get();

        if (pic)
        {
            // ✔ 只做降采样
            downscale_rgb565(
                (uint16_t*)pic->buf, 320, 320,
                (uint16_t*)cam_buf, CAM_W, CAM_H);

            // ✔ 限制 UI 刷新频率（关键）
            if (lv_tick_elaps(last_ui) > 50)
            {
                last_ui = lv_tick_get();

                if (lvgl_lock(-1))
                {
                    lv_img_set_src(objects.img_camera, &cam_img);
                    lvgl_unlock();
                }
            }

            // ✔ 拍照触发 AI（异步）
            if (capt_b && !face_busy)
            {
                capt_b = 0;
                // 拷贝一帧，避免被 camera_task 覆盖
                memcpy(cam_who_buf, pic->buf, pic->width * pic->height * 2);
                xTaskCreatePinnedToCore(
                    face_task,
                    "face_task",
                    1024 * 12,
                    NULL,
                    4,
                    NULL,
                    0
                );
            }

            esp_camera_fb_return(pic);
        }

        vTaskDelay(pdMS_TO_TICKS(30)); // ❗ 从1改到30
    }
}

static void lvgl_task(void *param)
{
    while (1)
    {
        uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        while (1)
        {
            // Lock the mutex due to the LVGL APIs are not thread-safe
            if (lvgl_lock(-1))
            {
                if (!face_busy)
                {
                    task_delay_ms = lv_timer_handler();
                    ui_tick();
                }
                else
                {
                    task_delay_ms = 50; // AI期间降低刷新
                }
                lvgl_unlock();
            }
            if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS)
            {
                task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
            }
            else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS)
            {
                task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
            }
            vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
        }
    }
}

void app_main(void)
{
    rgb888_buf = heap_caps_malloc(
    320 * 320 * 3,
    MALLOC_CAP_SPIRAM
    );
    lvgl_api_mux = xSemaphoreCreateRecursiveMutex();
    lv_init();
    camera_init();
    display_init();
    touch_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lvgl_tick_timer_init(EXAMPLE_LVGL_TICK_PERIOD_MS);
    bsp_brightness_init();
    bsp_brightness_set_level(80);
    if (lvgl_lock(-1))
    {
        ui_init(); // 初始化UI
        lvgl_unlock();
    }
    xTaskCreatePinnedToCore(lvgl_task, "bsp_lv_port_task", 1024 * 20, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(camera_task, "camera_task_task", 1024 * 3, NULL, 1, NULL, 0);
}


