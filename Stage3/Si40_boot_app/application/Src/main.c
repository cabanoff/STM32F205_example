/*
  STM32F205RB 
 *      Flash:           0x08000000 - 0x0801FFFF (128 KB)
 *      Bootloader:      0x08000000 - 0x08007FFF (32 KB)
 *      EEPROM:          0x08008000 - 0x0800FFFF (32 KB)
 *      Application:     0x08010000 - 0x08013FFF (16KB)

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
#include "tim1.h"
#include "gpio.h"


/** @addtogroup STM32F2xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_VER        2
#define APP_SUB_VER    3
#define APP_BUILD      51
#define APP_CHECK      (APP_VER + APP_SUB_VER + APP_BUILD)
/* Private macro -------------------------------------------------------------*/
extern uint32_t __checksum;                                 // import checksum
 
/* Private variables ---------------------------------------------------------*/

//uint32_t* ptr = begin;
volatile uint32_t *pCrc;
unsigned char version;

#pragma default_variable_attributes = @ ".version"
const unsigned char appVer [] = {APP_VER,APP_SUB_VER,APP_BUILD,APP_CHECK};
#pragma default_variable_attributes =
/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
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
  SystemClock_Config();
  
  gpioInit();  
  tim1Init();
  
  pCrc = &__checksum;      //to avoid optimization by compilator
  version = appVer[0];  // to avoid optimization by compilator
  
  //delay = 500;// delay



  /* -3- Toggle PA.0 IOs in an infinite loop */  
  while (1)
  {
    
    /* STANDBY Mode with RTC on LSE/LSI Entry 
    - RTC Clocked by LSE/LSI
    - Backup SRAM ON
    - IWDG OFF
    - Automatic Wakeup using RTC clocked by LSE/LSI (after ~20s)
    */
    gpioPWROn();    
    gpioPA2Off();
    tim1SetPeriod();    
    HAL_Delay(5);
    //gpioPA2On();
    
    tim1Start();
    HAL_Delay(30);
    tim1Stop();
    
    gpioPA2On();                //LED off
    gpioPWROff();
    
 //   HAL_Delay(20000);
    
    tim1DeInit();
    gpioDeInit();
    StandbyRTCBKPSRAMMode_Measure();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
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
     Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  /*
  //Configure the Systick interrupt time 
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  //Configure the Systick    
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  //SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  gpioPA2Off();
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
