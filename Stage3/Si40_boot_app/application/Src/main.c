/*
  STM32F205RB 
 *      Flash:           0x08000000 - 0x0801FFFF (128 KB)
 *      Bootloader:      0x08000000 - 0x08007FFF (32 KB)
 *      EEPROM:          0x08008000 - 0x0800FFFF (32 KB)
 *      Application:     0x08010000 - 0x08011FFF (8KB)

  ******************************************************************************
  * @file    Si40_boot_app\application\main.c 
  * @author  Kabanov A
  * @brief   Software for downloading via UART by bootloader
  *          
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <intrinsics.h>


/** @addtogroup STM32F2xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_VER        2
#define APP_SUB_VER    1
#define APP_BUILD      42
#define APP_CHECK      (APP_VER + APP_SUB_VER + APP_BUILD)
/* Private macro -------------------------------------------------------------*/
extern uint32_t __checksum;                                 // import checksum
 
/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef  GPIO_InitStruct;

//uint32_t* ptr = begin;
uint32_t delay = 1000;
volatile uint32_t *pCrc;
unsigned char version;

#pragma default_variable_attributes = @ ".version"
const unsigned char appVer [] = {APP_VER,APP_SUB_VER,APP_BUILD,APP_CHECK};
#pragma default_variable_attributes =
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */



// ------------------------------------------------------
int main(void)
{
    
   __enable_interrupt();
    
    /* STM32F2xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 120 MHz */
  //SystemClock_Config();
  
  /* -1- Enable GPIOA (to be able to program the configuration registers) */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* -2- Configure PA.0IOs in output push-pull mode to
         drive external LEDs */
  GPIO_InitStruct.Pin = (GPIO_PIN_0);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  pCrc = &__checksum;      //to avoid optimization by compilator
  version = appVer[0];  // to avoid optimization by compilator
  
  delay = 500;// delay



  /* -3- Toggle PA.0 IOs in an infinite loop */  
  while (1)
  {
    for(int i = 0; i < 20; i++)  //20 toggles of led
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
      /* Insert delay */
      HAL_Delay(delay);
    }
    /* STANDBY Mode with RTC on LSE/LSI Entry 
    - RTC Clocked by LSE/LSI
    - Backup SRAM ON
    - IWDG OFF
    - Automatic Wakeup using RTC clocked by LSE/LSI (after ~20s)
    */
    StandbyRTCBKPSRAMMode_Measure();
  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/*****************************END OF FILE****/
