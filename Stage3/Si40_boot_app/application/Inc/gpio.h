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
  * @brief  deinitializing all GPIOs
  * @param  None
  * @retval None
  */
void gpioDeInit(void);

void gpioPWROn(void);
void gpioPWROff(void);
void gpioPA2On(void);
void gpioPA2Off(void);

uint32_t gpioGetPA0(void);

#endif /* __GPIO_H */
