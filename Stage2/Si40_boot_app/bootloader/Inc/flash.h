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
  * @brief  erase sector 4 0x801 0000 - 0x801 FFFF
  * @param  None
  * @note   None
  *         
  * @retval 1 - successful erasure 0 - error during erasure
  */

uint8_t flashErase4Sector(void);
    
/**
  * @brief  fill flash memory with data in buffer
  * @param  buffer pointer to buffer
  *         length - data length
  * @note   None
  *         
  * @retval 1 - successful downloading 0 - error during downloading
  */

uint8_t flashFillMemory(uint32_t* buffer, uint32_t length);
/**
  * @brief  None
  * @param  None
  * @retval None
  */


#endif /* __FLASH_H */
