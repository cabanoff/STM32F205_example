/**
  ******************************************************************************
  * @file           : enterID.c
  * @brief          : all routines for entering ID
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "eeprom.h"
#include "flash.h"

/** @addtogroup ENTERID
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t ID;
  uint32_t Channel;
  uint32_t Mode;
  uint32_t summ;
} param_t;
typedef union
{
  param_t param;
  uint32_t eeprom[sizeof(param_t)/sizeof(uint32_t)];
} eeprom_t;
/* Private define ------------------------------------------------------------*/
#define SUMM_PARAM (eeprom.param.ID + eeprom.param.Channel + eeprom.param.Mode)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int counter = 0;
eeprom_t eeprom;
/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
/**
  * @brief  mode for entring device ID  consists of 4 digits
  *             write device ID into EEPROM
  * @param  data from terminal, accepts only 4 charscter if they are digits
  * @retval -1 exit from mode
  *          0 stay in mode 
  */
int eepromEnterID(unsigned char data)
{
  return -1 ;
}
/**
  * @brief  get device ID from eeprom
  *
  * @param  None
  * @retval -1 no ID found
  *          ID from 1 to 9999 
  */
int eepromGetID(void)
{
  if(flashReadEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  if(SUMM_PARAM != eeprom.param.summ) return -1;
  if((eeprom.param.ID > 9999)||(eeprom.param.ID == 0))return -1;
  return (int)eeprom.param.ID;
}
/**
  * @brief  get Channel number from eeprom
  *
  * @param  None
  * @retval -1 no Channel found
  *          Channel from 1 to 35 
  */
int eepromGetChannel(void)
{
  if(flashReadEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  if(SUMM_PARAM != eeprom.param.summ) return -1;
  if((eeprom.param.Channel > 35)||(eeprom.param.Channel == 0))return -1;
  return (int)eeprom.param.Channel;
}

/**
  * @brief  get Mode from eeprom
  *
  * @param  None
  * @retval -1 no Channel found
  *          Mode from 1 to 3 (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  *          if channels 31-35 returns 2 - NORMAL
  */
int eepromGetMode(void)
{
  if(flashReadEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  if(SUMM_PARAM != eeprom.param.summ) return -1;
  if((eeprom.param.Mode > 3)||(eeprom.param.Mode == 0))return -1;
  if(eeprom.param.Channel > 30)return 2;
  return (int)eeprom.param.Mode;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  * @param  None
  * @param  None
  * @retval None
  */


/**
  * @}
  */ 
