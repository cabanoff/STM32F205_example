/*
 * STM32F205RB 
 *      Flash:           0x08000000 - 0x0801FFFF (128 KB)
 *      Bootloader:      0x08000000 - 0x0800BFFF (48 KB)
 *      EEPROM:          0x0800C000 - 0x0800FFFF (16 KB)
 *      Application:     0x08010000 - 0x0801FFFF (64KB)


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
#include "intrinsics.h"

/* Private typedef -----------------------------------------------------------*/
typedef void (application_t) (void);

typedef struct vector
{
    uint32_t        stack_addr;     // intvec[0] is initial Stack Pointer
    application_t   *func_p;        // intvec[1] is initial Program Counter
} vector_t;

/* Private variables ---------------------------------------------------------*/
extern const uint32_t app_vector;   // Application vector address symbol from 
                                    // the linker configuration file
const vector_t *vector_p            = (vector_t*) &app_vector;
volatile uint32_t stack_arr[100]    = {0}; // Allocate some stack
                                               // just to show that
                                               // the SP should be reset
                                               // before the jump - or the
                                               // stack won't be configured
                                               // correctly.
uint32_t bufferDownload[100];

uint32_t length = COUNTOF(bufferDownload);
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
static void goToApp(void);
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

  for(uint32_t i = 0; i < length; i++)bufferDownload[i] = i;

  /* Configure the system clock */
  SystemClock_Config();

  gpioInit();
  uartInit(9600);
  
  /* Initialize interrupts */
  MX_NVIC_Init();
 
  gpioSetPC3();
  gpioClearPC3();              //to enable RX
  uartStartRX();               

  /* Infinite loop */

  while (1)
  {

    if(uartIsData())
    {
      char data = uartGetData();
      
      uartStartTX(data);
      switch(data)
      {
       case '1':         //eraze sector 4
        __no_operation();
       break;
      case '2':         //write buffer to sector 4
        flashFillMemory(bufferDownload,length);
        break;
      case '3':         //write buffer to sector 4
        goToApp();
        break;
      }
      
      uartStartRX(); 
    }

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
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
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
  * @brief  hand over application
  *
  * @retval None
  */
static void goToApp(void)
{
  HAL_Delay(1000);
  uartDeInit();
  gpioDeInit();
  HAL_DeInit();
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
