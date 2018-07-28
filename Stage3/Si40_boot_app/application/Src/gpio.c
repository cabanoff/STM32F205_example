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
   /* -1- Enable GPIOC Clock (to be able to program the configuration registers) */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  /* -1- Enable GPIOA Clock (to be able to program the configuration registers) */
  __HAL_RCC_GPIOA_CLK_ENABLE();
   /* -2- Configure PC.3, IOs in output push-pull mode to
         drive optocouple */   
  GPIO_InitStruct.Pin = (GPIO_PIN_3|GPIO_PIN_0);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  /* -2- Configure PA.2IOs in output push-pull mode to
         drive external LEDs */
  GPIO_InitStruct.Pin = (GPIO_PIN_2);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* -2- Configure PA.0IOs in input push-pull mode to
         check sensor */
  GPIO_InitStruct.Pin = (GPIO_PIN_0);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  
  /* GPIO TIM1_Channel1 & TIM1_Channel3 configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
/**
  * @brief  deinitializing all GPIOs
  * @param  None
  * @retval None
  */
void gpioDeInit(void)
{
  HAL_GPIO_DeInit(GPIOC, 0xFFFF);
  HAL_GPIO_DeInit(GPIOA, 0xFFFF);
  HAL_GPIO_DeInit(GPIOB, 0xFFFF);
}

/**
  * @brief  set GPIOC 0
  * @param  None
  * @retval None
  */

void gpioPWROn(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}
void gpioPWROff(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}
void gpioPA2On(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}
void gpioPA2Off(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

uint32_t gpioGetPA0(void)
{
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET)return 0;
  return 1; 
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
