/**
  ******************************************************************************
  * @file    gpio.h 
  * @author  AKabanov
  * @brief   Header for gpio.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENTERID_H
#define __ENTERID_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
  * @brief  mode for entring device ID  consists of 4 digits
  *             write device ID into EEPROM
  * called every time when key pressed
  * @param  data from terminal, accepts only 4 charscter if they are digits
  * @retval -1 exit from mode
  *          0 stay in mode 
  */
int enterID(unsigned char data);
#endif /* __ENTERID_H */
