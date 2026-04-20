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


static const char *TAG = "lvgl_example";

static lv_indev_drv_t indev_drv; // Input device driver (Touch)

static void task(void *param)
{
    // ESP_LOGI(TAG, "run");
    while (1)
    {
        uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        while (1)
        {
            // Lock the mutex due to the LVGL APIs are not thread-safe
            if (lvgl_lock(-1))
            {
                task_delay_ms = lv_timer_handler();
                ui_tick(); 
                // Release the mutex
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

static void camera_task(void *param);
void app_main(void)
{
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
    xTaskCreatePinnedToCore(task, "bsp_lv_port_task", 1024 * 20, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(camera_task, "camera_task_task", 1024 * 3, NULL, 1, NULL, 0);
}

#include "screens.h"
#define CAM_W 120
#define CAM_H 88

static lv_color_t cam_buf[CAM_W * CAM_H];
static lv_img_dsc_t cam_img = {
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .header.w = CAM_W,
    .header.h = CAM_H,
    .data_size = CAM_W * CAM_H * 2,
    .data = (uint8_t *)cam_buf,
};
static lv_color_t cam_cap_buf[CAM_W * CAM_H];
static lv_img_dsc_t cap_img = {
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .header.w = CAM_W,
    .header.h = CAM_H,
    .data_size = CAM_W * CAM_H * 2,
    .data = (uint8_t *)cam_cap_buf,
};
static lv_color_t cam_who_buf[CAM_W * CAM_H*4];
static lv_img_dsc_t who_img = {
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .header.w = CAM_W*2,
    .header.h = CAM_H*2,
    .data_size = CAM_W * CAM_H * 8,
    .data = (uint8_t *)cam_who_buf,
};

void action_camera_capture(lv_event_t * e)
{
    // ESP_LOGI(TAG, "width %d height %d",fb->width,fb->height);
    
    memcpy(cam_cap_buf,cam_buf, CAM_W * CAM_H*2);
    if (lvgl_lock(-1))
    {
        lv_img_set_src(objects.img_captue, &cap_img);
        lvgl_unlock();
    }
}

static void camera_task(void *param)
{
    camera_fb_t *pic;
    while (1)
    {
        pic = esp_camera_fb_get();

        if (NULL != pic)
        {
            downscale_rgb565((uint16_t*)pic->buf, 240, 176, (uint16_t*)cam_buf, CAM_W, CAM_H);
            if (lvgl_lock(-1))
            {
                lv_img_set_src(objects.img_camera, &cam_img);
                lvgl_unlock();
            }
            esp_camera_fb_return(pic);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    vTaskDelete(NULL);
}
