#ifndef __GUI_APP_H
#define __GUI_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define USER_COLOR lv_color_hex(0x034885)

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void gui_app(void);
lv_obj_t* get_text_message_label(void);
void set_log_in_btn_inactive(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __GUI_APP_H */
