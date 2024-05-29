/**
  ******************************************************************************
  * @file           : touchpad.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "touchpad.h"
#include "tft.h"
#include "stm32746g_discovery_ts.h"

led touchpad::f_led_green(LED1_GPIO_Port, LED1_Pin, led::conn::FORWARD);

void touchpad::init()
{
  for(static bool is_initialized = false; !is_initialized; is_initialized = true)
  {
    BSP_TS_Init(TFT_HOR_RES, TFT_VER_RES);

    lv_indev_t* input_device = lv_indev_create();
    lv_indev_set_type(input_device, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(input_device, read);
  }
}

/**
 * Read an input device
 * @param indev id of the input device to read
 * @param data put the x coordinate to data->point.x,
 *             put the y coordinate to data->point.y
 */
void touchpad::read(lv_indev_t* indev, lv_indev_data_t* data)
{
  static lv_point_t last_point = { 0, 0 };
  f_led_green.toggle();

  TS_StateTypeDef ts_state;
  BSP_TS_GetState(&ts_state);

  if (ts_state.touchDetected) 
  {
    data->point.x = ts_state.touchX[0];
    data->point.y = ts_state.touchY[0];
    last_point = data->point;
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else 
  {
    data->point = last_point;
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
