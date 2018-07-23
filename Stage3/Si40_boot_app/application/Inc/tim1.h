/**
  ******************************************************************************
  * @file    tim1.h 
  * @author  AKabanov
  * @brief   Header for tim1.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM1_H
#define __TIM1_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define Tx1_Pin GPIO_PIN_7
#define Tx1_GPIO_Port GPIOA
#define Tx2_Pin GPIO_PIN_8
#define Tx2_GPIO_Port GPIOA

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/**
  * @brief  init timer
  * @param  None
  * @retval None
  */

void tim1Init(void);
/**
  * @brief  stop timer
  * @param  None
  * @retval None
  */

void tim1Stop(void);
/**
  * @brief  start timer
  * @param  None
  * @retval None
  */
void tim1Start(void);
/**
  * @brief  set period 
  * @param  None
  * @retval true sucsess
  */
uint8_t tim1SetPeriod(void);
/**
  * @brief  temer deinitialization
  * @param  None
  * @retval None
  */

void tim1DeInit(void);

/**
  * @brief  get Mode from eeprom
  *
  * @param  None
  * @retval -1 no Mode found
  *          Mode from 1 to 3 (1 -  FAST, 2 - NORMAL, 3 - SLOW)
  *          if channels 31-35 returns 4 - Marport_NORMAL
  */
int eepromGetMode(void);

#endif /* __TIM1_H */
