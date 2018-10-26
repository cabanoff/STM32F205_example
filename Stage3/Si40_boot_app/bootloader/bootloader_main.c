/*
 * STM32F205RB 
 *      Flash:           0x08000000 - 0x0801FFFF (128 KB)
 *      Bootloader:      0x08000000 - 0x08007FFF (32 KB)
 *      EEPROM:          0x08008000 - 0x0800FFFF (32 KB)
 *      Application:     0x08010000 - 0x08013FFF (16KB)


  ******************************************************************************
  * @file    Si40_boot_app\bootloader\bootloader_main.c 
  * @author  Kabanov A
  * @brief   bootloader for Si40
  *          
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f2xx_hal.h"


#include "gpio.h"
#include "uart.h"
#include "flash.h"
#include "xmodem.h"
#include "eeprom.h"
#include "intrinsics.h"
#include "crc.h"
#include "alarm.h"

#define MAX_DOWNLOADED_KBYTES 16
#define MAX_DOWNLOAD_BYTES   (1024 * MAX_DOWNLOADED_KBYTES)
#define BOOT_VER        3
#define BOOT_SUB_VER    2
#define BOOT_BUILD      104
#define APP_VER        appVer.buffVer[0]
#define APP_SUB_VER    appVer.buffVer[1]
#define APP_BUILD      appVer.buffVer[2]
#define APP_CHECK      appVer.buffVer[3]
#define APP_SUMM      (APP_VER + APP_SUB_VER + APP_BUILD)
#define SWITCH_APP1      1000                                 //delay after switching on
#define SWITCH_APP2      (180UL*1000)                           //delay after pressing key


/* Private typedef -----------------------------------------------------------*/
typedef void (application_t) (void);

typedef struct vector
{
    uint32_t        stack_addr;     // intvec[0] is initial Stack Pointer
    application_t   *func_p;        // intvec[1] is initial Program Counter
} vector_t;


typedef union download
{  
  unsigned char   forXModem[MAX_DOWNLOAD_BYTES + 40];
  uint32_t        forFlash[MAX_DOWNLOAD_BYTES/4];
} download_t;

typedef enum
{
  initialMode,
  enterIDMode,
  enterChMode,
  enterModeMode,
} inputMode_t;

typedef union
{
  uint32_t uiVer;
  unsigned char buffVer[4];
} version_t;

/* Private variables ---------------------------------------------------------*/
extern const uint32_t app_vector;   // Application vector address symbol from 
                                    // the linker configuration file
const vector_t *vector_p            = (vector_t*) &app_vector;
//volatile uint32_t stack_arr[100]    = {0}; // Allocate some stack
                                               // just to show that
                                               // the SP should be reset
                                               // before the jump - or the
                                               // stack won't be configured
                                               // correctly.
download_t buffer;
int xmodemResult = 0;
static inputMode_t mode = initialMode;
volatile uint32_t sysTickCounter = 0;
char *errorFile;
int errorLine;
version_t appVer;
char data;
char debugData[256];
uint8_t debugDataptr = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
static void printDevInfo(void);
static void goToApp(void);
static void goToAppQuick(void);
int inbyte(unsigned short);
void outbyte(int);


/* Private functions ---------------------------------------------------------*/
// ------------------------------------------------------
/**
  * @brief  The application entry point
  *
  * @retval None
  */
int main(void)
{
    
   /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  //for(uint32_t i = 0; i < MAX_DOWNLOAD_BYTES/4; i++)buffer.forFlash[i] = 0x33;

  /* Configure the system clock */
  SystemClock_Config();
  
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Check and handle if the system was resumed from StandBy mode */ 
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    goToAppQuick();
    
  }
  HAL_Delay(500); //wait power to stabilaze
  
  gpioInit();
  gpioPWROn();
  uartInit(4800);
  xmodenInit(inbyte,outbyte);
  alarmInit();
  
  
  /* Initialize interrupts */
  MX_NVIC_Init();
  gpioTxEn();
  gpioLEDOn();
  gpioRedLEDOn();
  HAL_Delay(10);   
  alarmSet(SWITCH_APP1);
  alarmToggleSet(500);

  /* Output a message on Hyperterminal using printf function */
  printf("\n\r Start bootloader software");
  printDevInfo();
  //printf(" Press h for help\n\r"); 
  gpioRxEn();
  HAL_Delay(10);
  uartStartRX(); 
  //gpioPWROff();
  /* Infinite loop */

  while (1)
  {

    if(uartIsData())
    {      
      data = uartGetData();
      //if(((data >= '0')&&(data <= 'y'))||(data == 0x0a)||(data == 0x0d)||(data == ' '))alarmSet(SWITCH_APP2);
      debugData[debugDataptr] = data;
      debugDataptr++;
      gpioTxEn();
      HAL_Delay(1);
      if((data == 0x20)||(data == 0x0a)||(data == 0x0d))
      {
        alarmSet(SWITCH_APP2);
        if(data == 0x20) outbyte(0x08);  //acknowledgement
         HAL_Delay(100);
 //       HAL_Delay(300);              
 //       outbyte(0x20);  //acknowledgement
 //       HAL_Delay(100);
 //       outbyte('z');  //debug
      }
     
      if(mode == initialMode)
      {
        switch(data)
        {
          case 'd':         //download file using xmodem
            //printf("\n\r File>Send File...\n\r");
            gpioRxEn();
            HAL_Delay(10);
            xmodemResult = xmodemReceive(buffer.forXModem, MAX_DOWNLOAD_BYTES);
            gpioTxEn();
            alarmSet(SWITCH_APP2);  // prolong time
            HAL_Delay(10);
            if(xmodemResult < 0) // if something wrong has happend during downloading
            {
              switch(xmodemResult)
              {
                case -1:
                  printf("\n\r time out.\n\r");
                  break;
                case -2:
                  printf("\n\r inappropriate file.\n\r");
                  break;
              default:
                  printf("\n\r error %d .\n\r", xmodemResult);
              }
            }
            else
            {
              printf("\n\r read %d bytes.\n\r", xmodemResult);
              xmodemResult = crcCompare(buffer.forFlash,MAX_DOWNLOAD_BYTES/4 - 1,buffer.forFlash[(MAX_DOWNLOAD_BYTES/4)-1]); 
              appVer.uiVer = buffer.forFlash[(MAX_DOWNLOAD_BYTES/4)-2];  //read application version
              if((xmodemResult == 0)&&(APP_CHECK == APP_SUMM))
              {
                //printf(" Application version %d.%d build %d, press 'y' to write in Flash memory.\n\r", APP_VER, APP_SUB_VER, APP_BUILD);
                printf(" Application version %d.%d build %d.\n\r", APP_VER, APP_SUB_VER, APP_BUILD);
              }
              else printf("CRC or Version number of downladed file is not correct.\n\r");            
            }
            break;
          case 'y':
            xmodemResult = crcCompare(buffer.forFlash,MAX_DOWNLOAD_BYTES/4 - 1,buffer.forFlash[(MAX_DOWNLOAD_BYTES/4)-1]); 
            appVer.uiVer = buffer.forFlash[(MAX_DOWNLOAD_BYTES/4)-2];  //read application version
            if((xmodemResult == 0)&&(APP_CHECK == APP_SUMM))   //data in buffer is valid application code
            {
                xmodemResult  = flashFillMemory(buffer.forFlash,MAX_DOWNLOAD_BYTES/4,appSector);
                switch(xmodemResult)
                {
                case 0:
                  printf("\n\r Data was successfully written in Flash memory.\n\r");
                  break;
                case 1:
                  printf("\n\r Error occurred while Flash erase.\n\r ");
                  break;
                case 2:
                  printf("\n\r Error occurred while writing data in Flash memory.\n\r");
                  break;
                }
            }
            else printf("\n\r File doesn't contain valid application code.\n\r"); // data in buffer is not application code
            
          break;
          case 'j':         //jump from bootloader to application
            xmodemResult = crcCompare((uint32_t*)&app_vector, MAX_DOWNLOAD_BYTES/4 - 1, *(uint32_t*)(&app_vector + MAX_DOWNLOAD_BYTES/4 - 1));
            appVer.uiVer = *(uint32_t*)(&app_vector + MAX_DOWNLOAD_BYTES/4 - 2);
            if((xmodemResult == 0)&&(APP_CHECK == APP_SUMM))
            {
              goToApp();
            }
            else printf("\n\r Application doesn't exist.\n\r");
            break;
          case 'i':         
            //printf("\n\r Enter 4 digits of device ID, then press return.\n\r");
            eepromIDModePrep();
            mode = enterIDMode;
            break;
          case 'c':
            //printf("\n\r Enter frequency channel number(1-35), then press return.\n\r");
            eepromChModePrep();
            mode = enterChMode;
            break;
          case 'm':
            //printf("\n\r Enter mode - 1(FAST), 2(NORMAL), 3(SLOW), then press return.\n\r");
            mode = enterModeMode;
            break;
          case 'p':
            printDevInfo();
            break;  
          case 'h':
            printf("\n\r d - Download image");
            printf("\n\r j - Start application");
            printf("\n\r i - Enter Device ID");
            printf("\n\r m - Enter mode");
            printf("\n\r c - Enter channel");
            printf("\n\r p - Print device information");
            printf("\n\r return - check connection\n\r");
            break;
          case '\n':
          case '\r':
            printf(" connection at 4800 baud\n\r");
            break;
        }
      }
      else if (mode == enterIDMode)
      {
        int val = eepromEnterID(data);
        if( val == -1) //
        {
          mode = initialMode;
          printf("\n\r Error. \n\r");
        }
        if(val == 1)
        {
          mode = initialMode;
          printf("\n\r New device ID is %s \n\r", eepromIDString(eepromGetID()));
        }         
      }
      else if (mode == enterChMode)
      {
        int val = eepromEnterCh(data);
        if( val == -1) //
        {
          mode = initialMode;
          printf("\n\r Error. \n\r");
        }
        if(val == 1)
        {
          mode = initialMode;
          printf("\n\r New frequency channel is %s \n\r", eepromChannelString(eepromGetChannel()));
          printf("\n\r New frequency %s.\n\r",eepromFreqString(eepromGetChannel()));
        }        
        
      }
       else if (mode == enterModeMode)
      {
        int val = eepromEnterMode(data);
        if( val == -1) //
        {
          mode = initialMode;
          printf("\n\r Error. \n\r");
        }
        if(val == 1)
        {
          mode = initialMode;
          printf("\n\r New Mode is %s ,\n\r", eepromModeString(eepromGetMode())); 
          //printf(" but if the frequency channel is 31 - 35, mode is %s. \n\r",eepromModeString(4));
        }        
        
      }
      gpioRxEn();
      HAL_Delay(10);
      uartStartRX(); 
    }
    if(alarmIsAlarm())goToApp();
    if(alarmIsToggle())gpioRedLEDToggle();
  }
}

// ------------------------------------------------------
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 240
  *            PLL_P                          = 2
  *            PLL_Q                          = 5
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}



/**
  * @brief  rec
  * @param timeout - timeout in msec
  * @retval input byte from UART
  *         -1 if timeout expired
  */
int inbyte(unsigned short timeout)
{
  return (uartStartRXBlock(timeout));
}

void outbyte(int c)
{
  uint8_t outData = c;
  uartStartTXBlock(outData);
}

void printDevInfo(void)
{
  
  int ID = eepromGetID();
  if(ID < 0) printf("\n\r Device ID doesn't exist.\n\r");
  else printf("\n\r Device ID %s.\n\r",eepromIDString(ID));
  
  int Channel = eepromGetChannel();
  if(Channel < 0) printf(" Channel isn't set.\n\r");
  else 
  {
    printf(" Channel %s.\n\r",eepromChannelString(Channel));
    printf(" Frequency %s.\n\r",eepromFreqString(Channel));
  }
  if((Channel > 0)&&(Channel < 31))
  {
    int Mode = eepromGetMode();
    if(Mode < 0) printf(" Mode isn't set.\n\r");
    else printf(" %s mode .\n\r", eepromModeString(Mode));
  }
  if((Channel > 30)&&(Channel < 36))
  {
    printf(" %s mode .\n\r", eepromModeString(4));
  }
  appVer.uiVer = *(uint32_t*)(&app_vector + MAX_DOWNLOAD_BYTES/4 - 1);  //reading 
  printf("\n\r Bootloader version %d.%d build %d.\n\r", BOOT_VER, BOOT_SUB_VER, BOOT_BUILD);
  appVer.uiVer = *(uint32_t*)(&app_vector + MAX_DOWNLOAD_BYTES/4 - 2);
  if((crcCompare((uint32_t*)&app_vector, MAX_DOWNLOAD_BYTES/4 - 1, *(uint32_t*)(&app_vector + MAX_DOWNLOAD_BYTES/4 - 1)) == 0)&&
    (APP_CHECK == APP_SUMM))
  {
    printf(" Application version %d.%d build %d.\n\r", APP_VER, APP_SUB_VER, APP_BUILD);
  }
  else printf(" Application doesn't exist.\n\r");
}

/**
  * @brief  hand over application
  *
  * @retval None
  */
static void goToApp(void)
{
  printf("\n\r Exit from bootloader.\n\r");
  printf(" Go to application.\n\r");
  gpioLEDOff();
  gpioPWROff();
  HAL_Delay(1000); // delay for reguletion 
  uartDeInit();
  gpioDeInit();
  alarmDeInit();
  HAL_DeInit();
  __disable_interrupt();              // 1. Disable interrupts
  __set_SP(vector_p->stack_addr);     // 2. Configure stack pointer
  SCB->VTOR = (uint32_t) &app_vector; // 3. Configure VTOR
  vector_p->func_p();                 // 4. Jump to application
}
/**
  * @brief  hand over application in the case of wake up from standby mode
  *
  * @retval None
  */

static void goToAppQuick(void)
{
  __disable_interrupt();              // 1. Disable interrupts
  __set_SP(vector_p->stack_addr);     // 2. Configure stack pointer
  SCB->VTOR = (uint32_t) &app_vector; // 3. Configure VTOR
  vector_p->func_p();                 // 4. Jump to application
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */

static void MX_NVIC_Init(void)
{
  //USART1_IRQn interrupt configuration
  HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  //TIM1_CC_IRQn interrupt configuration
 // HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 0);
 // HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  //TIM1_UP_TIM10_IRQn interrupt configuration
 // HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 3, 0);
 // HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  //RCC_IRQn interrupt configuration
 // HAL_NVIC_SetPriority(RCC_IRQn, 4, 0);
 // HAL_NVIC_EnableIRQ(RCC_IRQn);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  errorFile = file;
  errorLine = line;
  while(1)
  {
    __no_operation();
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE****/
