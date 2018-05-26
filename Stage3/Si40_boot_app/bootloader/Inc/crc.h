/**
  ******************************************************************************
  * @file    crc.h 
  * @author  AKabanov
  * @brief   Header for crc.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC_H
#define __CRC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
  * @brief  crc compare 
  * @param  begin - address of buffer,
  *         length - length of buffer in bytes
  *         crc - precalculated crc
  * @retval 0 - crc corresponds
  *         -1 - doesn't corresspond
  */
int crcCompare(uint32_t* begin, uint32_t length, uint32_t crc);

#endif /* __CRC_H */
