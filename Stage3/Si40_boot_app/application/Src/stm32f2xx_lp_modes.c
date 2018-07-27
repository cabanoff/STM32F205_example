/**
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/stm32f2xx_lp_modes.c 
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the STM32F2xx Low Power Modes:
  *           - Sleep Mode
  *           - STOP mode with RTC
  *           - STANDBY mode without RTC and BKPSRAM
  *           - STANDBY mode with RTC
  *           - STANDBY mode with RTC and BKPSRAM
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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

/** @addtogroup STM32F2xx_HAL_Examples
  * @{
  */

/** @addtogroup PWR_CurrentConsumption
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;

/**
  * @brief  RTCHandler setup
  * 
  * @param  Data Data to be written in the sRTC_BKP_DR0.
  * @retval None
  */

void RTCInit(void)
{
  /* Configure RTC prescaler and RTC data registers as follow:
  - Hour Format = Format 24
  - Asynch Prediv = Value according to source clock
  - Synch Prediv = Value according to source clock
  - OutPut = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType = Open Drain */ 
  RTCHandle.Instance = RTC;
  RTCHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RTCHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RTCHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

  if(HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }

}

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the system to enter Standby mode with RTC 
  *         clocked by LSE or LSI and with Backup SRAM ON for current consumption 
  *         measurement purpose.
  *         STANDBY Mode with RTC clocked by LSE/LSI and BKPSRAM
  *         ====================================================
  *           - RTC Clocked by LSE or LSI
  *           - Backup SRAM ON
  *           - IWDG OFF
  *           - Automatic Wakeup using RTC clocked by LSE/LSI (after ~20s)
  * @param  None
  * @retval None
  */
void StandbyRTCBKPSRAMMode_Measure(int wakeUpTime)
{
  
  /*## Configure the Wake up timer ###########################################*/
  /*  RTC Wakeup Interrupt Generation:
      Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI))
      Wakeup Time = Wakeup Time Base * WakeUpCounter 
                  = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE or LSI)) * WakeUpCounter
      ==> WakeUpCounter = Wakeup Time / Wakeup Time Base

      To configure the wake up timer to 1s the WakeUpCounter is set to 0x800:
        RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16 
        Wakeup Time Base = 16 /(~32.768KHz) 
        Wakeup Time = 1s =   16 /(~32.768KHz) * WakeUpCounter
        ==> WakeUpCounter = 1s * 32768Hz/16 = 2048 = 0x800 */
  /* Disable Wake-up timer */
  HAL_RTCEx_DeactivateWakeUpTimer(&RTCHandle);
  
  /*## Clear all related wakeup flags ########################################*/
  /* Clear PWR wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  
  /* Clear RTC Wake Up timer Flag */
  __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTCHandle, RTC_FLAG_WUTF);
  
  /*## Setting the Wake up time ##############################################*/
  HAL_RTCEx_SetWakeUpTimer_IT(&RTCHandle,(uint32_t)wakeUpTime /*(0x800UL*34)-70*/, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

  /* Enable BKPRAM Clock */
  __HAL_RCC_BKPSRAM_CLK_ENABLE();

  /* Enable the Backup SRAM low power Regulator */
  HAL_PWREx_EnableBkUpReg();

  /*## Enter the Standby mode ################################################*/
  /* Request to enter STANDBY mode  */
  HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  Writes a data in RTC_BKP_DR0.
  * 
  * @param  Data Data to be written in the sRTC_BKP_DR0.
  * @retval None
  */
void BKUP0Write(unsigned int Data)
{
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR0, Data);
}

/**
  * @brief  Reads data from the RTC_BKP_DR0.
  * 
  * @retval Read value
  */
unsigned int BKUP0Read(void)
{
  return HAL_RTCEx_BKUPRead(&RTCHandle, RTC_BKP_DR0);
}
/**
  * @brief  Writes a data in RTC_BKP_DR1.
  * 
  * @param  Data Data to be written in the sRTC_BKP_DR1.
  * @retval None
  */
void BKUP1Write(unsigned int Data)
{
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR1, Data);
}

/**
  * @brief  Reads data from the RTC_BKP_DR1.
  * 
  * @retval Read value
  */
unsigned int BKUP1Read(void)
{
  return HAL_RTCEx_BKUPRead(&RTCHandle, RTC_BKP_DR1);
}
/**
  * @brief  Writes a data in RTC_BKP_DR2.
  * 
  * @param  Data Data to be written in the sRTC_BKP_DR0.
  * @retval None
  */
void BKUP2Write(unsigned int Data)
{
  HAL_RTCEx_BKUPWrite(&RTCHandle, RTC_BKP_DR2, Data);
}

/**
  * @brief  Reads data from the RTC_BKP_DR2.
  * 
  * @retval Read value
  */
unsigned int BKUP2Read(void)
{
  return HAL_RTCEx_BKUPRead(&RTCHandle, RTC_BKP_DR2);
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
