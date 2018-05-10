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
  uint32_t IDCheck;
  uint32_t ChannelCheck;
  uint32_t ModeCheck;
} param_t;
typedef union
{
  param_t param;
  uint32_t eeprom[sizeof(param_t)/sizeof(uint32_t)];
} eeprom_t;
/* Private define ------------------------------------------------------------*/
#define CHECK(x) (x + 100)
#define ID_SIZE  4
#define CH_SIZE  2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int counter = 0;
eeprom_t eeprom;
uint8_t IDBuffer[ID_SIZE];
uint8_t chBuffer[CH_SIZE];
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  write device ID to eeprom
  *
  * @param  None
  * @retval -1 error during writing
  *         0 - success
  */
int writeID(uint32_t ID);
/**
  * @brief  write channel number to eeprom
  *
  * @param  None
  * @retval -1 error during writing
  *         0 - success
  */
int writeChannel(uint32_t channel);

/* Public functions ----------------------------------------------------------*/

 /**
  * @brief  prepare buffer for entering ID mode
  *
  * @param  None
  * @retval None
  */

void eepromIDModePrep(void)
{
  //fill ID buffer with zeroes
  for(int i = 0; i < ID_SIZE; i++)IDBuffer[i] = 0;
}
/**
  * @brief  mode for entring device ID  consists of 4 digits
  *             write device ID into EEPROM
  * @param  data from terminal, accepts only 4 charscter if they are digits
  * @retval -1 exit from mode with error
  *          0 stay in mode 
  *          1 exit from mode with new ID
  */
int eepromEnterID(unsigned char data)
{
  if((data == '\r')||(data == '\n')) //enter new device ID
  {
    uint32_t ID = 0;
    int mult = 1;
    for(int i = 0; i < ID_SIZE ; i++)
    {
      ID += IDBuffer[ID_SIZE - 1 - i]*mult;
      mult *= 10;
    }
    if(writeID(ID) < 0)return -1;
    return 1;
  }
  else if ((data >= '0')&&(data <= '9'))
  {
    //buffer left shift 
    for(int i = 0; i < ID_SIZE-1; i++)IDBuffer[i] = IDBuffer[i+1];
    IDBuffer[ID_SIZE - 1] = data - '0';
  }
  else eepromIDModePrep();
  return 0;
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
  if(CHECK(eeprom.param.ID) != eeprom.param.IDCheck) return -1;
  if((eeprom.param.ID > 9999)||(eeprom.param.ID == 0))return -1;
  return (int)eeprom.param.ID;

}

 /**
  * @brief  prepare buffer for entering Channel mode
  *
  * @param  None
  * @retval None
  */

void eepromChModePrep(void)
{
  //fill ID buffer with zeroes
  for(int i = 0; i < CH_SIZE; i++)chBuffer[i] = 0;
}
/**
  * @brief  mode for entring Channel number from 1 to 35
  *             write Channel into EEPROM
  * @param  data from terminal, accepts only 2 charscter if they are digits
  * @retval -1 exit from mode with error
  *          0 stay in mode 
  *          1 exit from mode with new Channel number
  */
int eepromEnterCh(unsigned char data)
{
  if((data == '\r')||(data == '\n')) //enter new Channel number
  {
    uint32_t channel = 0;
    int mult = 1;
    for(int i = 0; i < CH_SIZE ; i++)
    {
      channel += chBuffer[CH_SIZE - 1 - i]*mult;
      mult *= 10;
    }
    if((channel > 35)||(channel == 0))return -1;
    if(writeChannel(channel) < 0)return -1;
    return 1;
  }
  else if ((data >= '0')&&(data <= '9'))
  {
    //buffer left shift 
    for(int i = 0; i < CH_SIZE-1; i++)chBuffer[i] = chBuffer[i+1];
    chBuffer[CH_SIZE - 1] = data - '0';
  }
  else eepromChModePrep();
  return 0;
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
  if(CHECK(eeprom.param.Channel) != eeprom.param.ChannelCheck) return -1;
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
  if(CHECK(eeprom.param.Mode) != eeprom.param.ModeCheck) return -1;
  if((eeprom.param.Mode > 3)||(eeprom.param.Mode == 0))return -1;
  if(eeprom.param.Channel > 30)return 2;
  return (int)eeprom.param.Mode;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  write device ID to eeprom
  *
  * @param  None
  * @retval -1 error during writing
  *         0 - success
  */
int writeID(uint32_t ID)
{
  if(flashReadEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  eeprom.param.ID = ID;
  eeprom.param.IDCheck = CHECK(ID);
  if(flashWriteEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  return 0;
}

/**
  * @brief  write channel number to eeprom
  *
  * @param  None
  * @retval -1 error during writing
  *         0 - success
  */
int writeChannel(uint32_t channel)
{
  if(flashReadEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  eeprom.param.Channel = channel;
  eeprom.param.ChannelCheck = CHECK(channel);
  if(flashWriteEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  return 0;
}
/**
  * @brief  
  * @param  None
  * @param  None
  * @retval None
  */


/**
  * @}
  */ 
