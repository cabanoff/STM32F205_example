/**
  ******************************************************************************
  * @file    alarm.h 
  * @author  AKabanov
  * @brief   Header for alarm.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALARM_H
#define __ALARM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
  * @brief  initializing alarm
  * @param  None
  * @retval None
  */
void alarmInit(void);

/**
  * @brief  deinitializing alarm
  * @param  None
  * @retval None
  */
void alarmDeInit(void);
/**
  * @brief  seting the time of alarm in ms from current time.
  * @param  time of alarm in ms
  * @retval None
  */
void alarmSet(uint32_t alarmTimeSet);
/**
  * @brief  check if alarm is triggered
  * @param  None
  * @retval 1 - alarme is triggered
  *         0 - alarme is not triggered
  */
uint8_t alarmIsAlarm(void);
/**
  * @brief  check if it is time to toggle
  * @param  None
  * @retval 1 - time to toggle
  *         0 - it's early yet
  */

void alarmToggleSet(uint32_t timeToggleSet);

/**
  * @brief  check if it is time to toggle
  * @param  None
  * @retval 1 - time to toggle
  *         0 - it's early yet
  */
uint8_t alarmIsToggle(void);


#endif /* __ALARM_H */
