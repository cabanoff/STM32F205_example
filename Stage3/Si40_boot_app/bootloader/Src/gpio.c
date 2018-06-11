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
  
  GPIO_InitStruct.Pin = (GPIO_PIN_8|GPIO_PIN_0);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  
}
/**
  * @brief  deinitializing all GPIOs
  * @param  None
  * @retval None
  */
void gpioDeInit(void)
{
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3|GPIO_PIN_0);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_8);
}
/**
  * @brief  set GPIOC 3, close U2
  * @param  None
  * @retval None
  */
void gpioTxEn(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

/**
  * @brief  clear GPIOC 3
  * @param  None
  * @retval None
  */
void gpioRxEn(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}

/**
  * @brief  clear GPIOA 0
  * @param  None
  * @retval None
  */
void gpioLEDOff(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}

/**
  * @brief  set GPIOA 0
  * @param  None
  * @retval None
  */
void gpioLEDOn(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
}

void gpioPWROn(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}
void gpioPWROff(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}
void gpioRedLEDOff(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

void gpioRedLEDOn(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}

void gpioRedLEDToggle(void)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
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
