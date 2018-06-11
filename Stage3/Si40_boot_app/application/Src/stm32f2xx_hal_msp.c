/**
  ******************************************************************************
  * File Name          : stm32f2xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

//extern void _Error_Handler(char *, int);
/* USER CODE BEGIN 0 */

/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  * 
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *        the RTC clock source; in this case the Backup domain will be reset in  
  *        order to modify the RTC Clock source, as consequence RTC registers (including 
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *             
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /*##-1- Configue LSE as RTC clock soucre ###################################*/ 
    
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    Error_Handler();
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    Error_Handler();
  }

  
  /*##-2- Enable the RTC peripheral Clock ####################################*/ 
  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE();
  
  /*##-3- Configure the NVIC for RTC Alarm ###################################*/
  HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}


/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

  /* Peripheral interrupt init */

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

//void HAL_RNG_MspInit(RNG_HandleTypeDef* hrng)
//{
//
//  if(hrng->Instance==RNG)
//  {
//  /* USER CODE BEGIN RNG_MspInit 0 */
//
//  /* USER CODE END RNG_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_RNG_CLK_ENABLE();
//  /* USER CODE BEGIN RNG_MspInit 1 */
//
//  /* USER CODE END RNG_MspInit 1 */
//  }
//
//}

//void HAL_RNG_MspDeInit(RNG_HandleTypeDef* hrng)
//{
//
//  if(hrng->Instance==RNG)
//  {
//  /* USER CODE BEGIN RNG_MspDeInit 0 */
//
//  /* USER CODE END RNG_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_RNG_CLK_DISABLE();
//  /* USER CODE BEGIN RNG_MspDeInit 1 */
//
//  /* USER CODE END RNG_MspDeInit 1 */
//  }
//
//}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM1_CLK_ENABLE();
    
  /* Enable GPIO Channels Clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  //__HAL_RCC_GPIOB_CLK_ENABLE();
  //__HAL_RCC_GPIOE_CLK_ENABLE();  
  
  /* Configure (PA.08, TIM1_CH1), (PA.07, TIM1_CH1N)
  in output, push-pull, alternate function mode  */
  
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  
  /* GPIO TIM1_Channel1 & TIM1_Channel3 configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
}

//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
//{
//
//  GPIO_InitTypeDef GPIO_InitStruct;
//  if(htim->Instance==TIM1)
//  {
//  //USER CODE BEGIN TIM1_MspPostInit 0 */
//
//  //USER CODE END TIM1_MspPostInit 0 */
//  
//    /**TIM1 GPIO Configuration    
//    PA7     ------> TIM1_CH1N
//    PA8     ------> TIM1_CH1 
//    */
//    GPIO_InitStruct.Pin = Tx1_Pin|Tx2_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  //USER CODE BEGIN TIM1_MspPostInit 1 */
//
//  //USER CODE END TIM1_MspPostInit 1 */
//  }
//
//}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{

  if(htim_pwm->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /* TIM1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }

}

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
//void HAL_UART_MspInit(UART_HandleTypeDef* huart)
//{
//
//  GPIO_InitTypeDef GPIO_InitStruct;
//  if(huart->Instance==USART1)
//  {
//  /* USER CODE BEGIN USART1_MspInit 0 */
//    /* Enable GPIO TX/RX clock */
//   __HAL_RCC_GPIOB_CLK_ENABLE();
//  /* USER CODE END USART1_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_USART1_CLK_ENABLE();
//  
//    /**USART1 GPIO Configuration    
//    PB6     ------> USART1_TX
//    PB7     ------> USART1_RX 
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN USART1_MspInit 1 */
//
//  /* USER CODE END USART1_MspInit 1 */
//  }
//
//}

//void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
//{
//
//  if(huart->Instance==USART1)
//  {
//  /* USER CODE BEGIN USART1_MspDeInit 0 */
//
//  /* USER CODE END USART1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USART1_CLK_DISABLE();
//  
//    /**USART1 GPIO Configuration    
//    PB6     ------> USART1_TX
//    PB7     ------> USART1_RX 
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);
//
//    /* USART1 interrupt DeInit */
//    HAL_NVIC_DisableIRQ(USART1_IRQn);
//  /* USER CODE BEGIN USART1_MspDeInit 1 */
//
//  /* USER CODE END USART1_MspDeInit 1 */
//  }
//
//}

//void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
//{
//
//  if(hwwdg->Instance==WWDG)
//  {
//  /* USER CODE BEGIN WWDG_MspInit 0 */
//
//  /* USER CODE END WWDG_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_WWDG_CLK_ENABLE();
//  /* USER CODE BEGIN WWDG_MspInit 1 */
//
//  /* USER CODE END WWDG_MspInit 1 */
//  }
//
//}

/* USER CODE BEGIN 1 */
/**
  * @brief RTC MSP De-Initialization 
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
   __HAL_RCC_RTC_DISABLE();     
}


/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
