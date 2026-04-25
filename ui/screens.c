#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 2, 0, e);
    }
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        action_camera_start(e);
    }
}

static void event_handler_cb_camera_obj1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 0, 0, e);
    }
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        action_camera_end(e);
    }
}

static void event_handler_cb_camera_obj2(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        action_capture_delete(e);
    }
}

static void event_handler_cb_camera_obj3(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        action_camera_capture(e);
    }
}

void create_screen_main() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_photo1);
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 84, 240);
            lv_obj_set_size(obj, 73, 32);
            lv_obj_add_event_cb(obj, event_handler_cb_main_obj0, LV_EVENT_ALL, flowState);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0cb490), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "start");
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
}

void create_screen_camera() {
    void *flowState = getFlowState(0, 1);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.camera = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2e2727), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 169, 274);
            lv_obj_set_size(obj, 60, 40);
            lv_obj_add_event_cb(obj, event_handler_cb_camera_obj1, LV_EVENT_ALL, flowState);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd8e6e9), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff026166), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "break");
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 10, 274);
            lv_obj_set_size(obj, 60, 40);
            lv_obj_add_event_cb(obj, event_handler_cb_camera_obj2, LV_EVENT_ALL, flowState);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff427191), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe60d0d), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj4 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffbc2b2b), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "del");
                }
            }
        }
        {
            // img_camera
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.img_camera = obj;
            lv_obj_set_pos(obj, 0, -2);
            lv_obj_set_size(obj, 240, 240);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 16, 248);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Count:");
        }
        {
            // t_num
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.t_num = obj;
            lv_obj_set_pos(obj, 104, 248);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff96dc19), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 90, 274);
            lv_obj_set_size(obj, 60, 40);
            lv_obj_add_event_cb(obj, event_handler_cb_camera_obj3, LV_EVENT_ALL, flowState);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0cb490), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffe60d0d), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "capt");
                }
            }
        }
        {
            // box_0
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.box_0 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 0, 50 },
                { 50, 50 },
                { 50, 0 },
                { 0, 0 }
            };
            lv_line_set_points(obj, line_points, 5);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xfff5f600), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // box_1
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.box_1 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 0, 50 },
                { 50, 50 },
                { 50, 0 },
                { 0, 0 }
            };
            lv_line_set_points(obj, line_points, 5);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xfff5f600), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // box_2
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.box_2 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 0, 50 },
                { 50, 50 },
                { 50, 0 },
                { 0, 0 }
            };
            lv_line_set_points(obj, line_points, 5);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xfff5f600), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // box_3
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.box_3 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 0, 50 },
                { 50, 50 },
                { 50, 0 },
                { 0, 0 }
            };
            lv_line_set_points(obj, line_points, 5);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xfff5f600), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // box_4
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.box_4 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 0, 50 },
                { 50, 50 },
                { 50, 0 },
                { 0, 0 }
            };
            lv_line_set_points(obj, line_points, 5);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xfff5f600), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // box_5
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.box_5 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 0, 50 },
                { 50, 50 },
                { 50, 0 },
                { 0, 0 }
            };
            lv_line_set_points(obj, line_points, 5);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xfff5f600), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_camera();
}

void tick_screen_camera() {
    void *flowState = getFlowState(0, 1);
    (void)flowState;
}


static const char *screen_names[] = { "Main", "camera" };
static const char *object_names[] = { "main", "camera", "obj0", "obj1", "obj2", "obj3", "obj4", "img_camera", "t_num", "box_0", "box_1", "box_2", "box_3", "box_4", "box_5" };


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_camera,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_camera();
}
