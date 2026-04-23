#pragma once

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    int class_id;
    float score;
} detect_box_t;
#define MAX_BOX 10
extern detect_box_t g_boxes[MAX_BOX];
extern int g_box_num;

void coco_detect_run(
    uint8_t *rgb565,
    uint16_t w,
    uint16_t h
);

