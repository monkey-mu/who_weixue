#include "coco_detect.hpp"
#include "dl_image_jpeg.hpp"
// extern "C" {
// #include "init_board.h"
// }


const char *TAG = "yolo11n";
extern "C" void coco_detect_run(uint8_t *rgb565, uint16_t w, uint16_t h, uint8_t *detect_img)
{
    ESP_LOGI(TAG,"coco begin");
    dl::image::img_t img = {.data=rgb565, .width=w, .height=h, .pix_type=dl::image::DL_IMAGE_PIX_TYPE_RGB888};
    COCODetect *detect = new COCODetect();
    ESP_LOGI(TAG,"heap internal: %d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    ESP_LOGI(TAG,"heap psram   : %d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    auto &detect_results = detect->run(img);
    for (const auto &res : detect_results) {
        ESP_LOGI(TAG,
                 "[category: %d, score: %f, x1: %d, y1: %d, x2: %d, y2: %d]",
                 res.category,
                 res.score,
                 res.box[0],
                 res.box[1],
                 res.box[2],
                 res.box[3]);
    }
    ESP_LOGI(TAG,"coco over");
    delete detect;

 //   memcpy(detect_img,img.data, img.width*img.height*2);

    heap_caps_free(img.data);
}