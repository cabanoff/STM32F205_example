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

extern void Error_Handler(void);

/** @addtogroup TIM1
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*channels 0 - 29 PI-C, 30 - 34, MARPORT*/
uint32_t PWMPeriods[] = {2751,
                         2739,2726,2714,2702,2690,2678,
                         2666,2654,2642,2631,2619,2608,
                         2596,2585,2574,2563,2552,2541,
                         2531,2520,2509,2499,2489,2478,
                         2468,2458,2448,2438,2428,
                         2777,2783,2790,2770,2764};
eeprom_t eeprom;
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

#define FLASH_EEPROM_START_ADDR ((uint32_t)0x08008000)  /* Start @ of eeprom area */
#define CHECK(x) (x + 100)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
uint8_t flashReadEEPROM(uint32_t* buffer, uint32_t length);
int eepromGetChannel(void);
/* Private functions ---------------------------------------------------------*/
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

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  get Mode from eeprom
  *
  * @param  None
  * @retval -1 no Mode found
  *          Mode from 1 to 3 (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  *          if channels 31-35 returns 4 - Marport_NORMAL
  */
int eepromGetMode(void)
{
  int channel;
  
  
  if(flashReadEEPROM(eeprom.eeprom , sizeof(eeprom)/sizeof(uint32_t)) != 0)return -1;
  if(CHECK(eeprom.param.Mode) != eeprom.param.ModeCheck) return -1;
  if((eeprom.param.Mode > 3)||(eeprom.param.Mode == 0))return -1;
  channel = eepromGetChannel();
  if((channel > 30)&&(channel < 36))return 4;
  return (int)eeprom.param.Mode;
}

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
   Error_Handler(); 
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
    Error_Handler(); 
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
    Error_Handler(); 
  }
  /*##-3- Start PWM signals generation #######################################*/ 
  /* Start channel 1 */
  //tim1Start();
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
    Error_Handler(); 
  }
  /* Start channel 1N */
  if(HAL_TIMEx_PWMN_Stop(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler(); 
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
    Error_Handler(); 
  }
  /* Start channel 1N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler(); 
  }    
}

/**
  * @brief  set period 
  * @param  None
  * @retval 1 sucsess
  */
int period;
uint8_t tim1SetPeriod(void)
{
  period = eepromGetChannel();
  if(period > 0)period = PWMPeriods[period - 1];
  else period = MAX_PERIOD_VALUE;
  
  if(TimHandle.Instance != TIM1)return 0;
  TimHandle.Init.Period            = period;
  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  /* Set the pulse value for channel 1 */
  sPWMConfig.Pulse = period/2;  
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler(); 
  }
  return 1;
}
/**
  * @brief  temer deinitialization
  * @param  None
  * @retval None
  */

void tim1DeInit(void)
{
  HAL_TIM_PWM_MspDeInit(&TimHandle);
}

/**
  * @}
  */ 
