/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f2xx_hal.h"

/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "uart.h"
#include "tim1.h"
#include "intrinsics.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
/*
RNG_HandleTypeDef hrng;

WWDG_HandleTypeDef hwwdg;
*/
typedef struct 
{
  char digit;
  uint32_t period;
} PWMPeriodTypeDef;

#define MAX_PERIOD_NUM 10

PWMPeriodTypeDef  PWMPeriod[MAX_PERIOD_NUM] = {'0',2790,'1',2783,'2',2777,'3',2770,'4',2764,'5',2751,'6',2739,'7',2726,'8',2714,'9',2702};//2690,2678,2666,2608,2552,2499,2448,2438,2428

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//static void MX_USART1_UART_Init(void);
//static void MX_TIM1_Init(void);
//static void MX_RNG_Init(void);
//static void MX_WWDG_Init(void);
static void MX_NVIC_Init(void);
                                    
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  gpioInit();
  uartInit(9600);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();
 // MX_USART1_UART_Init();
  tim1Init();
 // MX_RNG_Init();
 // MX_WWDG_Init();
  

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  gpioSetPC3();
  gpioClearPC3();              //to enable RX
  uartStartRX();               
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    if(uartIsData())
    {
      char data = uartGetData();
      for(int i = 0; i < MAX_PERIOD_NUM; i++)
      {
        if(PWMPeriod[i].digit == data)
        {
          tim1Stop();
          tim1SetPeriod(PWMPeriod[i].period);
          tim1Start();
          break;
        }
      }
      uartStartRX(); 
    }

  }
  /* USER CODE END 3 */

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
    _Error_Handler(__FILE__, __LINE__);
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
    _Error_Handler(__FILE__, __LINE__);
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

/* RNG init function */
/*
static void MX_RNG_Init(void)
{

  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
*/

/* WWDG init function */
/*
static void MX_WWDG_Init(void)
{

  hwwdg.Instance = WWDG;
  hwwdg.Init.Prescaler = WWDG_PRESCALER_1;
  hwwdg.Init.Window = 64;
  hwwdg.Init.Counter = 64;
  hwwdg.Init.EWIMode = WWDG_EWI_DISABLE;
  if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
*/
/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
//static void MX_GPIO_Init(void)
//{
//
//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOH_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//
//}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



