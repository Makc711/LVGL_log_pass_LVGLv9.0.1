/**
 * @file flash_rw.h
 *
 */

#ifndef FLASF_RW_H
#define FLASF_RW_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "stm32f7xx_hal.h"

/*********************
 *      DEFINES
 *********************/
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 32 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08008000) /* Base address of Sector 1, 32 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08010000) /* Base address of Sector 2, 32 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08018000) /* Base address of Sector 3, 32 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08020000) /* Base address of Sector 4, 128 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08040000) /* Base address of Sector 5, 256 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08080000) /* Base address of Sector 6, 256 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x080C0000) /* Base address of Sector 7, 256 Kbytes */

/**********************
 * GLOBAL PROTOTYPES
 **********************/
uint32_t erase_sector(const uint32_t address_start, const uint32_t address_end); // Use before write!
void write_byte(const uint32_t address, const uint8_t byte);
uint8_t read_byte(const uint32_t address);
void check_byte(const uint32_t address, const uint8_t byte);
void write_str_to_flash(const char* str, const uint32_t address);
size_t read_str_from_flash(const uint32_t address, char* str, const size_t max_length);

#ifdef __cplusplus
}
#endif

#endif
