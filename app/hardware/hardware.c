/**
 * @file hardware.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "hardware.h"
#include "stm32746g_discovery.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void hardware_init(void)
{
    BSP_LED_Init(LED_GREEN);
}
