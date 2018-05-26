/**
  ******************************************************************************
  * @file    uart.h 
  * @author  AKabanov
  * @brief   Header for uart.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */

/* Size of Transmission buffer */
#define TXSTARTMESSAGESIZE                   (COUNTOF(aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                     (COUNTOF(aTxEndMessage) - 1)
#define TXBUFFERSIZE                    10

/* Size of Reception buffer */
#define RXBUFFERSIZE                      1
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */
/**
  * @brief  uart initialisation
  * @param  boudRate integer baud rate
  * @retval None
  */
void uartInit(uint32_t baudRate);
/**
  * @brief  uart start RX
  *     Put UART peripheral in reception process  
  *     Any data received will be stored "aRxBuffer" buffer : the number max of 
  *     data received is RXBUFFERSIZE
  * @param  None
  * @retval None
  */
void uartStartRX(void);
/**
  * @brief  uart start RX in blocking mode
  *     Put UART peripheral in reception process  
  *     Any data received will be stored aRxBuffer[0]
  * @param  Timeout in msec
  * @retval -1 if timeout  
  *         received data
  */
int uartStartRXBlock(uint32_t Timeout);
/**
  * @brief  uart start TX
  *     Put UART peripheral in transmission process  
  *     
  * @param  data data to transmit
  * @retval None
  */   
void uartStartTX(uint8_t data);
/**
  * @brief  uart start TX in blocking mode
  *     Put UART peripheral in transmission process  
  *     
  * @param  data data to transmit
  * @retval None
  */
void uartStartTXBlock(uint8_t data);
/**
  * @brief  check if data received from UART
  * @param  None
  * @retval 1 - data received 0 - no data
  */
uint8_t uartIsData(void);
/**
  * @brief  get data received from UART
  * @param  None
  * @retval data received
  */
char uartGetData(void);
/**
  * @brief  deinitialization uart before going to application
  * @param  None
  * @retval None
  */
void uartDeInit(void);

#endif /* __UART_H */
