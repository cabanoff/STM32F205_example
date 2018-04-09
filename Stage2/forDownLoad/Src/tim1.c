/**
  ******************************************************************************
  * @file    tim1.c 
  * @author  AKabanov
  * @brief   PWM functions
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim1.h"
#include "intrinsics.h"

extern void _Error_Handler(char *, int);

/** @addtogroup TIM1
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef               TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef              sPWMConfig;
/* Timer Break Configuration Structure declaration */
TIM_BreakDeadTimeConfigTypeDef sBreakConfig;

/* Counter Prescaler value */
uint32_t uwPrescalerValue = 0;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define  PERIOD_VALUE       2790  /* Period Value (2791 - 1) */
#define  PULSE1_VALUE       1395  /* Capture Compare 1 Value  */ //
#define  DEAD_TIME          0xc5
#define  MAX_PERIOD_VALUE   2790

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ---------------------------------------------------------*/

/**
  * @brief   
  ##-1- Configure the TIM peripheral #######################################
   ---------------------------------------------------------------------------
  1/ Generate complementary PWM signal :
  
    TIM1 input clock (TIM1CLK) is set to 2 * APB2 clock (PCLK2), since APB2 
    prescaler is different from 1.   
    TIM1CLK = 2 * PCLK2  
    PCLK2 = HCLK / 2 
    => TIM1CLK = 2 * (HCLK / 2) = HCLK = SystemCoreClock
  
    TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is set to have
    TIM1 counter clock = 120MHz..


    The Three Duty cycles are computed as the following description: 

    The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
    
    
    The Timer pulse is calculated as follows:
      - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

  2/ Insert a dead time equal to (312/SystemCoreClock) ns

  3/ Configure the break feature, active at High level, and using the automatic 
     output enable feature
  * @param  None
  * @note  
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f2xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency   
  --------------------------------------------------------------------------- 
  * @retval None
  */
void tim1Init(void)
{

  /* Select the Timer instance */
  TimHandle.Instance = TIM1;
  
  TimHandle.Init.Prescaler         = 0;
  TimHandle.Init.Period            = PERIOD_VALUE;
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;//TIM_AUTORELOAD_PRELOAD_ENABLE;
    
  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /*##-2- Configure the PWM channels #########################################*/ 
  /* Common configuration for all channels */
  sPWMConfig.OCMode       = TIM_OCMODE_PWM1;
  sPWMConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sPWMConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sPWMConfig.OCIdleState  = TIM_OCIDLESTATE_SET;
  sPWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;  

  /* Set the pulse value for channel 1 */
  sPWMConfig.Pulse = PULSE1_VALUE;  
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  
 
  
  /* Set the Break feature & Dead time */
  sBreakConfig.BreakState       = TIM_BREAK_DISABLE;//TIM_BREAK_ENABLE;//
  sBreakConfig.DeadTime         = DEAD_TIME;
  sBreakConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
  sBreakConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakConfig.LockLevel        = TIM_LOCKLEVEL_OFF;//TIM_LOCKLEVEL_1;//
  sBreakConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_ENABLE;//TIM_AUTOMATICOUTPUT_DISABLE;
  
  if(HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sBreakConfig) != HAL_OK)
  {
    /* Configuration Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  /*##-3- Start PWM signals generation #######################################*/ 
  /* Start channel 1 */
  tim1Start();
}
/**
  * @brief  stop timer
  * @param  None
  * @retval None
  */

void tim1Stop(void)
{   
  if(HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  /* Start channel 1N */
  if(HAL_TIMEx_PWMN_Stop(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    _Error_Handler(__FILE__, __LINE__);
  }    
}

/**
  * @brief  start timer
  * @param  None
  * @retval None
  */

void tim1Start(void)
{   
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  /* Start channel 1N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    _Error_Handler(__FILE__, __LINE__);
  }    
}

/**
  * @brief  set period 
  * @param  Period in 1/120MHz  units
  * @retval 1 sucsess
  */
uint8_t tim1SetPeriod(uint32_t period)
{
  if(period > MAX_PERIOD_VALUE)period = MAX_PERIOD_VALUE;
  if(TimHandle.Instance != TIM1)return 0;
  TimHandle.Init.Period            = period;
  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  /* Set the pulse value for channel 1 */
  sPWMConfig.Pulse = period/2;  
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    _Error_Handler(__FILE__, __LINE__);
  }
  return 1;
}

/**
  * @}
  */ 