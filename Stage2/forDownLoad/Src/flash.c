/**
  ******************************************************************************
  * @file    uart.c 
  * @author  AKabanov
  * @brief   all routines connected with code downloading into flash memory
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "flash.h"
#include "intrinsics.h"

extern void _Error_Handler(char *, int);

/** @addtogroup FLASH
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_CODE_START_ADDR   ((uint32_t)0x08010000)  /* Start @ of code Flash area */
#define FLASH_USER_END_ADDR     ((uint32_t)0x0801FFFF)/* End @ of code Flash area : sector start address + sector size - 1 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
uint32_t SectorError = 0;
uint32_t * pointer = NULL;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  None
  * @param  None
  * @note   None
  *         
  * @retval None
  */

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  erase sector 4 0x801 0000 - 0x801 FFFF
  * @param  None
  * @note   None
  *         
  * @retval 1 - successful erasure 0 - error during erasure
  */

uint8_t flashErase4Sector(void)
{
  return 1;
}

/**
  * @brief  fill flash memory with data in buffer
  * @param  buffer pointer to buffer
  *         length - data length
  * @note   None
  *         
  * @retval 0 - successful downloading
  *         1 - error during erasing
  *         2 - error during downloading
  *         3 - buffer to large
  */

uint8_t flashFillMemory(uint32_t* buffer, uint32_t length)
{
  uint8_t errorCode = 0;
  if(length == 0)return errorCode;
  if(length > 0xFFFFUL/4)return 3; //
  pointer = buffer;
  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();
  FirstSector = 4;
  NbOfSectors = 1;
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    /* 
      Error occurred while sector erase. 
      User can add here some code to deal with this error. 
      SectorError will contain the faulty sector and then to know the code error on this sector,
      user can call function 'HAL_FLASH_GetError()'
    */
    HAL_FLASH_Lock(); 
    return 1;
  }
  /* Program the user Flash area word by word
    (area defined by FLASH_CODE_START_ADDR and length of the buffer) ***********/

  Address = FLASH_CODE_START_ADDR;
  while (Address < (FLASH_CODE_START_ADDR + length*4))
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *pointer) == HAL_OK)
    {
      Address = Address + 4;
      pointer++;
    }
    else
    { 
      /* Error occurred while writing data in Flash memory. 
         User can add here some code to deal with this error */
      HAL_FLASH_Lock(); 
      return 2;
    }
  }
  
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock(); 
  
  /* Check if the programmed data is OK 
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_CODE_START_ADDR;
  pointer = buffer;
  MemoryProgramStatus = 0;
  while (Address < (FLASH_CODE_START_ADDR + length*4))
  {
    data32 = *(__IO uint32_t*)Address;

    if (data32 != *pointer)
    {
      MemoryProgramStatus++;  
    }

    Address = Address + 4;
    pointer++;
  }  
  if(MemoryProgramStatus) return 2;
  return 0;
}
/**
  * @}
  */ 