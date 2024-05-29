/**
  ******************************************************************************
  * @file           : flash_f7.cpp
  * @author         : Rusanov M.N.
  ******************************************************************************
  */

#include "flash_f7.h"

flash_f7::flash_f7() = default;

/**
  * @brief  Get the structure for sector erase.
  * @address_start[in]  sector start address
  * @address_end[in]    sector end address
  * 
  * @erase_init_struct[out]  A reference to the structure to erase, which will
  *                          be initialized here.
  */
void flash_f7::init_erase_struct(FLASH_EraseInitTypeDef &erase_init_struct, const uint32_t address_start, const uint32_t address_end)
{
  const uint32_t first_sector  = get_sector(address_start);
  const uint32_t nb_of_sectors = get_sector(address_end) - first_sector + 1;

  erase_init_struct.TypeErase     = FLASH_TYPEERASE_SECTORS;
  erase_init_struct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  erase_init_struct.Sector        = first_sector;
  erase_init_struct.NbSectors     = nb_of_sectors;
}

/**
  * @brief  Gets the sector of a given address
  * @param  address - destination address
  * @retval The sector of a given address
  */
uint32_t flash_f7::get_sector(const uint32_t address)
{
  uint32_t sector;

  if ((address < static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_1)) && (address >= static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_0)))
  {
    sector = FLASH_SECTOR_0;
  }
  else if ((address < static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_2)) && (address >= static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_1)))
  {
    sector = FLASH_SECTOR_1;
  }
#if (FLASH_SECTOR_TOTAL > 2)
  else if ((address < static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_3)) && (address >= static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_2)))
  {
    sector = FLASH_SECTOR_2;
  }
  else if ((address < static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_4)) && (address >= static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_3)))
  {
    sector = FLASH_SECTOR_3;
  }
#if (FLASH_SECTOR_TOTAL > 4)
  else if ((address < static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_5)) && (address >= static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_4)))
  {
    sector = FLASH_SECTOR_4;
  }
  else if ((address < static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_6)) && (address >= static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_5)))
  {
    sector = FLASH_SECTOR_5;
  }
  else if ((address < static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_7)) && (address >= static_cast<uint32_t>(addr::ADDR_FLASH_SECTOR_6)))
  {
    sector = FLASH_SECTOR_6;
  }
  else /* (address < FLASH_END_ADDR) && (address >= ADDR_FLASH_SECTOR_7) */
  {
    sector = FLASH_SECTOR_7;
  }
#endif
#endif
  return sector;
}