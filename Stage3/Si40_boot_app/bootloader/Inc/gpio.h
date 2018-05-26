/**
  ******************************************************************************
  * @file    gpio.h 
  * @author  AKabanov
  * @brief   Header for gpio.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/**
  * @brief  initializing all GPIOs
  * @param  None
  * @retval None
  */
void gpioInit(void);
/**
  * @brief  set GPIOC 3, close U2
  * @param  None
  * @retval None
  */
void gpioTxEn(void);
/**
  * @brief  clear GPIOC 3, open U2
  * @param  None
  * @retval None
  */
void gpioRxEn(void);
/**
  * @brief  deinitializing all GPIOs
  * @param  None
  * @retval None
  */
void gpioDeInit(void);

/**
  * @brief  clear GPIOA 0
  * @param  None
  * @retval None
  */
void gpioLEDOff(void);

/**
  * @brief  set GPIOA 0
  * @param  None
  * @retval None
  */
void gpioLEDOn(void);
void gpioPWROn(void);
void gpioPWROff(void);
void gpioRedLEDOff(void);
void gpioRedLEDOn(void);
void gpioRedLEDToggle(void);
#endif /* __GPIO_H */
