#include "coco_detect.hpp"
#include "dl_image_jpeg.hpp"
extern "C" {
#include "init_board.h"
}

extern const uint8_t bus_jpg_start[] asm("_binary_bus_jpg_start");
extern const uint8_t bus_jpg_end[] asm("_binary_bus_jpg_end");
const char *TAG = "yolo11n";

static lv_color_t cam_who_buf[240 * 176*2];
static lv_img_dsc_t who_img = {
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .header.w = 240,
    .header.h = 176,
    .data_size = 240 * 176 * 2,
    .data = (uint8_t *)cam_who_buf,
};
extern "C" void coco_detect_run(uint8_t *rgb565, int w, int h)
{
    l::image::jpeg_img_t jpeg_img = {.data = (void *)rgb565, .data_len = (size_t)(w*h*2)};
    auto img = dl::image::sw_decode_jpeg(jpeg_img, dl::image::DL_IMAGE_PIX_TYPE_RGB565LE);

    COCODetect *detect = new COCODetect();
    auto &detect_results = detect->run(img);

    delete detect;

    memcpy(cam_who_buf,img.data, img.bytes);
    if (lvgl_lock(-1))
    {
        lv_img_set_src(objects.img_captue, &who_img);
        lvgl_unlock();
    }

    heap_caps_free(img.data);
}