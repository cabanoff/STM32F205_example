/**
  ******************************************************************************
  * @file    eeprom.h 
  * @author  AKabanov
  * @brief   routines of save and restore parameters in EEPROM
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

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
int eepromEnterID(unsigned char data);
/**
  * @brief  
  *
  * @param  data from terminal, accepts only 4 charscter if they are digits
  * @retval -1 no ID found
  *          ID from 0 to 9999 
  */
int eepromGetID(void);
/**
  * @brief  get Channel number from eeprom
  *
  * @param  None
  * @retval -1 no Channel found
  *          Channel from 1 to 35 
  */
int eepromGetChannel(void);
    /**
  * @brief  get Mode from eeprom
  *
  * @param  None
  * @retval -1 no Channel found
  *          Mode from 1 to 3 (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  *          if channels 31-35 returns 2 - NORMAL
  */
int eepromGetMode(void);
#endif /* __EEPROM_H */
