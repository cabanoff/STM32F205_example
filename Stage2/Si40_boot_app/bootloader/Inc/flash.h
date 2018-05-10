/**
  ******************************************************************************
  * @file    uart.h 
  * @author  AKabanov
  * @brief   Header for flash.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef    enum
{
  bootSector = 1,
  eepromSector = 3,
  appSector = 4,
} sector_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */

/* Exported functions ------------------------------------------------------- */

 /**
  * @brief  fill flash memory with data in buffer
  * @param  buffer pointer to buffer
  *         length - data length in words(4 bytes)
  * @note   None
  *         
  * @retval 0 - successful downloading
  *         1 - error during erasing
  *         2 - error during downloading
  *         3 - buffer to large
  *         4 - not valid sector
  */
uint8_t flashFillMemory(uint32_t* buffer, uint32_t length,sector_t sector);
 /**
  * @brief  read EEPROM area
  * @param  buffer for data read
  *         length number of words to read
  * @note   read from 0x8008000 
  *         
  * @retval 0 - successful reading
  *         1 - error during reading
  *         
  */
uint8_t flashReadEEPROM(uint32_t* buffer, uint32_t length);
/**
  * @brief  write EEPROM area
  * @param  buffer for data written
  *         length number of words to write
  * @note   write to area starting from 0x80080000
  *         
  * @retval 0 - successful writing
  *         1 - error during writing
  *         
  */
uint8_t flashWriteEEPROM(uint32_t* buffer, uint32_t length);


#endif /* __FLASH_H */
