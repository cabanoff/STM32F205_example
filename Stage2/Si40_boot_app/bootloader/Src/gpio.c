/**
  ******************************************************************************
  * @file           : gpio.c
  * @brief          : all routines connected with gpio
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/** @addtogroup GPIO
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef  GPIO_InitStruct;

/* Private function prototypes -----------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
/**
  * @brief  initializing all GPIOs
  * @param  None
  * @retval None
  */
void gpioInit(void)
{
   /* -1- Enable GPIOG, GPIOC and GPIOI Clock (to be able to program the configuration registers) */
  __HAL_RCC_GPIOC_CLK_ENABLE();
   /* -2- Configure PC.3, IOs in output push-pull mode to
         drive optocouple */
  GPIO_InitStruct.Pin = (GPIO_PIN_3);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
}
/**
  * @brief  deinitializing all GPIOs
  * @param  None
  * @retval None
  */
void gpioDeInit(void)
{
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);
}
/**
  * @brief  set GPIOC 3
  * @param  None
  * @retval None
  */
void gpioSetPC3(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

/**
  * @brief  clear GPIOC 3
  * @param  None
  * @retval None
  */
void gpioClearPC3(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  * @param  None
  * @param  None
  * @retval None
  */


/**
  * @}
  */ 
