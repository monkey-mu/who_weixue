#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *camera;
    lv_obj_t *start;
    lv_obj_t *btn_break;
    lv_obj_t *btn_captue;
    lv_obj_t *img_edit;
    lv_obj_t *img_camera;
    lv_obj_t *t_num;
    lv_obj_t *box_0;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_CAMERA = 2,
};

void create_screen_main();
void tick_screen_main();

void create_screen_camera();
void tick_screen_camera();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/