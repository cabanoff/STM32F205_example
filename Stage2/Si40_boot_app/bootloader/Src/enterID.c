/**
  ******************************************************************************
  * @file           : enterID.c
  * @brief          : all routines for entering ID
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "enterID.h"

/** @addtogroup ENTERID
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int counter = 0;
unsigned char ID[4] = {"1234"};
/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
/**
  * @brief  mode for entring device ID  consists of 4 digits
  *             write device ID into EEPROM
  * @param  data from terminal, accepts only 4 charscter if they are digits
  * @retval -1 exit from mode
  *          0 stay in mode 
  */
int enterID(unsigned char data)
{
  return -1 ;
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
