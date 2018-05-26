/**
  ******************************************************************************
  * @file    crc.c  
  * @author  Kabanov A.
  * @brief   This code uses the STM32F2xx CRC HAL API 
  *          to get a CRC code of a given buffer of data word(32-bit), 
  *          based on a fixed generator polynomial(0x4C11DB7).
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "crc.h"

extern void _Error_Handler(char *, int);

/** @addtogroup CRC
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRC handler declaration */
CRC_HandleTypeDef   CrcHandle;

/* Used for storing CRC Value */
__IO uint32_t uwCRCValue = 0;
uint32_t calculatedCrc = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  crc compare 
  * @param  begin - address of buffer,
  *         length - length of buffer in bytes
  *         crc - precalculated crc
  * @retval 0 - crc corresponds
  *         -1 - doesn't corresspond
  */
uint32_t* debBegin;
uint32_t debLength;
uint32_t debCRC;
int crcCompare(uint32_t* begin, uint32_t length, uint32_t crc)
{
  debBegin = begin;
  debLength = length;
  debCRC = crc;
 /*##-1- Configure the CRC peripheral #######################################*/
  CrcHandle.Instance = CRC; 
  if(HAL_CRC_Init(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  /*##-2- Compute the CRC of "aDataBuffer" ###################################*/
  calculatedCrc= HAL_CRC_Calculate(&CrcHandle,begin,length);
  uwCRCValue = crc;
  if(HAL_CRC_DeInit(&CrcHandle) != HAL_OK)
  {
    /* Initialization Error */
    _Error_Handler(__FILE__, __LINE__);
  }    
  /*##-3- Compare the CRC value to the Expected one ##########################*/
  if(calculatedCrc == uwCRCValue) return 0;
  else return -1;
}
  
  
/**
  * @}
  */ 

/*****************************END OF FILE****/
