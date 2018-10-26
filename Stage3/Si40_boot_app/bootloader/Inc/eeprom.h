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
  * @brief  returms string with frequency 
  *
  * @param  Channel number Channel from 1 to 35 
  * @retval String with frequency, or with error
  *          
  */
char* eepromFreqString(int channel);

 /**
  * @brief  prepare buffer for entering ID mode
  *
  * @param  None
  * @retval None
  */
void eepromIDModePrep(void);

/**
  * @brief  mode for entring Channel number from 1 to 35
  *             write Channel into EEPROM
  * @param  data from terminal, accepts only 2 charscter if they are digits
  * @retval -1 exit from mode with error
  *          0 stay in mode 
  *          1 exit from mode with new Channel number
  */
int eepromEnterCh(unsigned char data);
    
 /**
  * @brief  prepare buffer for entering Channel mode
  *
  * @param  None
  * @retval None
  */

void eepromChModePrep(void);

/**
  * @brief  mode for entring Mode (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  *             write Channel into EEPROM
  * @param  data from terminal, accepts only 1 last charscter if it is a digit
  * @retval -1 exit from mode with error
  *          0 stay in mode 
  *          1 exit from mode with new Channel number
  */
int eepromEnterMode(unsigned char data);
/**
  * @brief  returms string with mode 
  *
  * @param  mode from 1 to 3 (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  * @retval String with mode 
  *          
  */

char* eepromModeString(int mode);
/**
  * @brief  prepare for entering Mode mode
  *
  * @param  None
  * @retval None
  */
void eepromModeModePrep(void);
/**
  * @brief  get Mode from eeprom
  *
  * @param  None
  * @retval -1 no Channel found
  *          Mode from 1 to 3 (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  *          if channels 31-35 returns 2 - NORMAL
  */
int eepromGetMode(void);
/**
  * @brief  re
  *
  * @param  device ID
  * @retval 4 digits string containing device ID like 0001, 0110 etc
  */
char* eepromIDString(int id);
/**
  * @brief  returns string with channel information
  *
  * @param  dchannel number
  * @retval string with channel
  */
char* eepromChannelString(int channel);

#endif /* __EEPROM_H */
