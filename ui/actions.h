#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {  
#endif

extern void action_camera_capture(lv_event_t * e);
extern void action_capture_delete(lv_event_t * e);
extern void action_camera_end(lv_event_t * e);
extern void action_camera_start(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/