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
eeprom_t eeprom;
static uint8_t IDBuffer[ID_SIZE];
static uint8_t chBuffer[CH_SIZE];
static uint8_t mode;
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
/**
  * @brief  write mode to eeprom
  *
  * @param  None
  * @retval -1 error during writing
  *         0 - success
  */
int writeMode(uint32_t mode);

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
  * @brief  returms string with frequency 
  *
  * @param  Channel number Channel from 1 to 35 
  * @retval String with frequency, or with error
  *          
  */
char freq [][8] ={{"no freq"},   // 0
                  {"43600Hz"},  // 1
                  {"43800Hz"},   // 2
                  {"44000Hz"},  // 3
                  {"44200Hz"},  // 4
                  {"44400Hz"},  // 5
                  {"44600Hz"},  // 6
                  {"44800Hz"},  // 77
                  {"45000Hz"},  // 8
                  {"45200Hz"},  // 9
                  {"45400Hz"},  // 10
                  {"45600Hz"},  // 11
                  {"45800Hz"},  // 12
                  {"46000Hz"},  // 13 
                  {"46200Hz"},  // 14
                  {"46400Hz"},  // 15
                  {"46600Hz"},  // 16 
                  {"46800Hz"},  // 17
                  {"47000Hz"},  // 18
                  {"47200Hz"},  // 19
                  {"47400Hz"},  // 20
                  {"47600Hz"},  // 21
                  {"47800Hz"},  // 22
                  {"48000Hz"},  // 23
                  {"48200Hz"},  // 24
                  {"48400Hz"},  // 25
                  {"48600Hz"},  // 26
                  {"48800Hz"},  // 27
                  {"49000Hz"},  // 28
                  {"49200Hz"},  // 29
                  {"49400Hz"},  // 30
                  {"43200Hz"},  // 31
                  {"43100Hz"},  // 32
                  {"43000Hz"},  // 33
                  {"43300Hz"},  // 34
                  {"43400Hz"}}; // 35

char errorChannel [] = {"No frequency"};
char* eepromFreqString(int channel)
{
  if((channel > 0)&&(channel < 36)) return freq[channel];
  else return errorChannel;
}
/**
  * @brief  returns string with device ID
  *
  * @param  device ID
  * @retval 4 digits string containing device ID like 0001, 0110 etc
  */

char* eepromIDString(int id)
{
  static char idString[] = "0000";
  static char errorString[] = "no ID";
  if((id < 0)||(id > 9999))return errorString;
  idString[0]  = id/1000 + '0';
  idString[1] = (id%1000)/100 + '0';
  idString[2] = (id%100)/10 + '0';
  idString[3] = (id%10) + '0';
  return idString;
}

/**
  * @brief  returns string with channel information
  *
  * @param  dchannel number
  * @retval string with channel
  */
char BoatCode [][20] = {{"BoatCode/Ch: C1/Ch6"},   // 31
                       {"BoatCode/Ch: C2/Ch6"},   // 32
                       {"BoatCode/Ch: C3/Ch6"},   // 33
                       {"BoatCode/Ch: C4/Ch6"},   // 34
                       {"BoatCode/Ch: C5/Ch6"}};  // 35
char* eepromChannelString(int channel)
{
  static char errorString[] = "no Channel";
  static char channelString[] = "Ch 00";
  if((channel < 1)||(channel > 35))return errorString;
  if(channel < 31)
  {
    channelString[3] = channel/10 + '0';
    channelString[4] = channel%10 + '0';
    return channelString;
  }
  return BoatCode[channel - 31];
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
  return (int)eeprom.param.Mode;
}
/**
  * @brief  returms string with mode 
  *
  * @param  mode from 1 to 3 (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  * @retval String with mode 
  *          
  */
char stringMode[][17] ={{"no mode"},   // 0
                        {"PI_FAST"},
                        {"PI_NORMAL"},
                        {"PI_SLOW"},
                        {"Marport"}};
char errorMode [] = {"No Mode selected"};
char* eepromModeString(int inpMode)
{
  if((inpMode > 0)&&(inpMode < 5)) return stringMode[inpMode];
  else return errorMode;
}
/**
  * @brief  prepare for entering Mode mode
  *
  * @param  None
  * @retval None
  */
void eepromModeModePrep(void)
{
  mode = 0;
}
/**
  * @brief  mode for entring Mode (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  *             write Channel into EEPROM
  * @param  data from terminal, accepts only 1 last charscter if it is a digit
  * @retval -1 exit from mode with error
  *          0 stay in mode 
  *          1 exit from mode with new Channel number
  */
int eepromEnterMode(unsigned char data)
{
  if((data == '\r')||(data == '\n')) //enter new mode
  {
    if((mode > 3)||(mode == 0))return -1;
    if(writeMode(mode) < 0)return -1;
    return 1;
  }
  else if ((data >= '1')&&(data <= '3'))mode = data - '0';
  else eepromModeModePrep();
  return 0;
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
  * @brief  write mode to eeprom
  *
  * @param  None
  * @retval -1 error during writing
  *         0 - success
  */
int writeMode(uint32_t inpMode)
{
  if(flashReadEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  eeprom.param.Mode = inpMode;
  eeprom.param.ModeCheck = CHECK(inpMode);
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
