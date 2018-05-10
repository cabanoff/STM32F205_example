/**
  ******************************************************************************
  * @file    flash.c 
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
#define FLASH_CODE_END_ADDR     ((uint32_t)0x0801FFFF)/* End @ of code Flash area : sector start address + sector size - 1 */
#define FLASH_EEPROM_START_ADDR ((uint32_t)0x08008000)  /* Start @ of eeprom area */
#define FLASH_EEPROM_END_ADDR   ((uint32_t)0x0800FFFF)/* End @ of code Flash area : sector start address + sector size - 1 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
  * @brief  read EEPROM area
  * @param  buffer for data read
  *         length number of words to read
  * @note   read from 0x8008000 
  *         
  * @retval 0 - successful reading
  *         1 - error during reading
  *         
  */
uint8_t flashReadEEPROM(uint32_t* buffer, uint32_t length)
{
  uint32_t Address = FLASH_EEPROM_START_ADDR;
  if(length == 0)return 0;
  if(length > 0x4000UL/4)return 1; //
  while (Address < (FLASH_EEPROM_START_ADDR + length*4))
  {
    *buffer = *(uint32_t*)Address;
    Address = Address + 4;
    buffer++;
  }  
  return 0;
}

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
uint8_t flashWriteEEPROM(uint32_t* buffer, uint32_t length)
{
  if(flashFillMemory(buffer, length, eepromSector )!=0)return 1;
  return 0;
}

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

uint8_t flashFillMemory(uint32_t* buffer, uint32_t length, sector_t sector )
{
  uint8_t errorCode = 0;
  uint32_t startAddr, maxLength;
  uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
  uint32_t SectorError = 0;
  uint32_t * pointer = NULL;
  uint32_t data32 = 0 , MemoryProgramStatus = 0;
  
  if(length == 0)return errorCode;
  pointer = buffer;
  
  if(sector == eepromSector)
  {
    FirstSector = 2;
    startAddr = FLASH_EEPROM_START_ADDR;
    maxLength = 0x4000UL/4;
  }
  else if(sector == appSector)
  {
    FirstSector = 4;
    startAddr = FLASH_CODE_START_ADDR;
    maxLength = 0xFFFFUL/4;
  }
  else return 4;
  if(length > maxLength)return 3; //
  
  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();
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

  Address = startAddr;
  while (Address < (startAddr + length*4))
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
  Address = startAddr;
  pointer = buffer;
  MemoryProgramStatus = 0;
  while (Address < (startAddr + length*4))
  {
    data32 = *(uint32_t*)Address;

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
