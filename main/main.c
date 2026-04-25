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
#define CAM_W 240
#define CAM_H 240


static volatile int capt_b = 0;
static volatile int camera_end_b = 0;
static volatile int face_busy = 0;
static volatile int camera_busy = 0;
lv_color_t *cam_buf;
static lv_img_dsc_t cam_img;



static void camera_task(void *param);
void action_camera_capture(lv_event_t * e)
{
    if (!face_busy) {
        capt_b = 1;
    }
}

void action_capture_delete(lv_event_t * e)
{
    if(!camera_busy){
        xTaskCreatePinnedToCore(camera_task, "camera_task_task", 1024 * 3, NULL, 1, NULL, 0);
    }
}

void action_camera_start(lv_event_t * e)
{
    if(!camera_busy){
        xTaskCreatePinnedToCore(camera_task, "camera_task_task", 1024 * 3, NULL, 1, NULL, 0);
    }
}

void action_camera_end(lv_event_t * e)
{
    if(camera_busy){
        camera_end_b = 1;
    }
}

//uint8_t *rgb565_buf;

static void face_task(void *param)
{
    lv_point_t points[5][5];
    char numbuf[6];
    face_busy = 1;

    // AI 推理（耗时）

    coco_detect_run(
        (uint8_t*)cam_buf,
        CAM_W,
        CAM_H
    );
    if(g_box_num>0)
    {
        for(int i=0;i<g_box_num&&i<5;i++)
        {
            points[i][0].x = g_boxes[i].x1;
            points[i][0].y = g_boxes[i].y1;
            points[i][1].x = g_boxes[i].x1;
            points[i][1].y = g_boxes[i].y2;
            points[i][2].x = g_boxes[i].x2;
            points[i][2].y = g_boxes[i].y2;
            points[i][3].x = g_boxes[i].x2;
            points[i][3].y = g_boxes[i].y1;
            points[i][4].x = g_boxes[i].x1;
            points[i][4].y = g_boxes[i].y1;
        }
        
    }

    itoa(g_box_num, numbuf,10);
    if (lvgl_lock(-1))
    {
        lv_line_set_points(objects.box_0,&points[0][0],5);         
        lv_line_set_points(objects.box_1,&points[1][0],5); 
        lv_line_set_points(objects.box_2,&points[2][0],5); 
        lv_line_set_points(objects.box_3,&points[3][0],5); 
        lv_line_set_points(objects.box_4,&points[4][0],5); 
        lv_label_set_text(objects.t_num,numbuf);
        lvgl_unlock();
    }

    face_busy = 0;
    vTaskDelete(NULL);
}


static void camera_task(void *param)
{
    camera_busy = 1;
    camera_fb_t *pic;
    uint32_t last_ui = 0;

    while (1)
    {
        pic = esp_camera_fb_get();

        if (pic)
        {
            memcpy(cam_buf, pic->buf, pic->width * pic->height * 2);
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
                // 拷贝一帧，避免被 camera_task 覆盖
                xTaskCreatePinnedToCore(
                    face_task,
                    "face_task",
                    1024 * 4,
                    NULL,
                    4,
                    NULL,
                    0
                );
            }

            esp_camera_fb_return(pic);
        }
        if(camera_end_b || capt_b)
        {
            capt_b = 0;
            camera_end_b = 0;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(30)); // ❗ 从1改到30
    }
    camera_busy = 0;
    vTaskDelete(NULL);
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
                    task_delay_ms = 500; // AI期间降低刷新
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
        
    cam_buf = heap_caps_malloc(CAM_W * CAM_H * 2, MALLOC_CAP_SPIRAM);
    cam_img.header.cf = LV_IMG_CF_TRUE_COLOR;
    cam_img.header.w = CAM_W;
    cam_img.header.h = CAM_H;
    cam_img.data_size = CAM_W * CAM_H * 2;
    cam_img.data = (uint8_t *)cam_buf;

    if (lvgl_lock(-1))
    {
        ui_init(); // 初始化UI
        lvgl_unlock();
    }
    xTaskCreatePinnedToCore(lvgl_task, "bsp_lv_port_task", 1024 * 6, NULL, 5, NULL, 1);
}


