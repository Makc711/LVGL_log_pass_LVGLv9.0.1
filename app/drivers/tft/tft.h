/**
 * @file tft.h
 *
 */

#ifndef DISP_H
#define DISP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      DEFINES
 *********************/
#define TFT_HOR_RES 480
#define TFT_VER_RES 272

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void tft_init(void);

#ifdef __cplusplus
}
#endif

#endif
