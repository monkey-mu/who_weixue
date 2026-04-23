#include "human_face_detect.hpp"
#include "dl_image_jpeg.hpp"
extern "C" {
#include "coco_detect.h"
}

detect_box_t g_boxes[MAX_BOX];
int g_box_num = 0;
const char *TAG = "yolo11n";
extern "C" void coco_detect_run(uint8_t *rgb565, uint16_t w, uint16_t h)
{
    dl::image::img_t img_src = {.data=rgb565, .width=w, .height=h, .pix_type =dl::image::DL_IMAGE_PIX_TYPE_RGB565BE};
    dl::image::img_t img_dst = {.data=heap_caps_malloc(160 * 120 * 3,MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT), 
                                .width=160, .height=120, .pix_type =dl::image::DL_IMAGE_PIX_TYPE_RGB888};
    if (!img_dst.data) {
    ESP_LOGI(TAG,"img_dst alloc failed\n");
    return;
    }
    dl::image::ImageTransformer transformer;
    transformer.set_src_img(img_src).set_dst_img(img_dst).set_src_img_crop_area({8, 12, 168, 132});
    transformer.transform();
    HumanFaceDetect  *detect = new HumanFaceDetect();
    auto &detect_results = detect->run(img_dst);
    g_box_num = 0;
    for (const auto &res : detect_results) { 
        g_boxes[g_box_num].x1 = res.box[0];
        g_boxes[g_box_num].y1 = res.box[1];
        g_boxes[g_box_num].x2 = res.box[2];
        g_boxes[g_box_num].y2 = res.box[3];
        g_box_num++;
    }
    ESP_LOGI(TAG,"coco over");
    delete detect;

    heap_caps_free(img_dst.data);
}