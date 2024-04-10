/**
 * @file touchpad.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "touchpad.h"
#include "lvgl/lvgl.h"
#include "tft.h"

#include "stm32746g_discovery.h"
#include "stm32746g_discovery_ts.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void touchpad_read(lv_indev_t* indev, lv_indev_data_t* data);

/**********************
 *  STATIC VARIABLES
 **********************/
static TS_StateTypeDef TS_State;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize your input devices here
 */
void touchpad_init(void)
{
  BSP_TS_Init(TFT_HOR_RES, TFT_VER_RES);

  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchpad_read);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Read an input device
 * @param indev id of the input device to read
 * @param data put the x coordinate to data->point.x,
 *             put the y coordinate to data->point.y
 */
static void touchpad_read(lv_indev_t* indev, lv_indev_data_t* data)
{
  /* Read your touchpad */
  static lv_point_t last_point = { 0, 0};
  BSP_LED_Toggle(LED1);

  BSP_TS_GetState(&TS_State);
  if(TS_State.touchDetected) 
  {
    data->point.x = TS_State.touchX[0];
    data->point.y = TS_State.touchY[0];
    last_point = data->point;
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else 
  {
    data->point = last_point;
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
