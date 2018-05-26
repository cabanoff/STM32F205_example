/**
  ******************************************************************************
  * @file           : alarm.c
  * @brief          : all routines connected with gpio
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "alarm.h"

/** @addtogroup ALARM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint32_t currentTime;
static uint32_t alarmTime, toggleTime, togglePeriod;
/* Private function prototypes -----------------------------------------------*/
void HAL_SYSTICK_Callback(void);

/* Public functions ----------------------------------------------------------*/
/**
  * @brief  initializing alarm
  * @param  None
  * @retval None
  */
void alarmInit(void)
{
   currentTime = 0;
   alarmTime = 0xFFFFFFFFUL;
}
/**
  * @brief  deinitializing alarm
  * @param  None
  * @retval None
  */
void alarmDeInit(void)
{
  
}
/**
  * @brief  seting the time of alarm in ms from current time.
  * @param  time of alarm in ms
  * @retval None
  */
void alarmSet(uint32_t alarmTimeSet)
{
  alarmTime = alarmTimeSet + currentTime;
}
/**
  * @brief  check if alarm is triggered
  * @param  None
  * @retval 1 - alarme is triggered
  *         0 - alarme is not triggered
  */
uint8_t alarmIsAlarm(void)
{
  if (currentTime > alarmTime) return 1;
  else return 0;
}

/**
  * @brief  check if it is time to toggle
  * @param  None
  * @retval 1 - time to toggle
  *         0 - it's early yet
  */

void alarmToggleSet(uint32_t timeToggleSet)
{
  togglePeriod = timeToggleSet;
  toggleTime = currentTime + togglePeriod;
  
}
/**
  * @brief  check if it is time to toggle
  * @param  None
  * @retval 1 - time to toggle
  *         0 - it's early yet
  */
uint8_t alarmIsToggle(void)
{
  if (currentTime > toggleTime)
  {
    toggleTime = currentTime + togglePeriod;
    return 1;
  }
  return 0;
}
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SYSTICK callback.
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSTICK_Callback could be implemented in the user file
   */
  currentTime++;
  
}
/**
  * @}
  */ 
