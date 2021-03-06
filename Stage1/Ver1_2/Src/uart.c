/**
  ******************************************************************************
  * @file    uart.c 
  * @author  AKabanov
  * @brief   all routines connected with usart1
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "intrinsics.h"

extern void _Error_Handler(char *, int);

/** @addtogroup UART
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t testRX = 0,testTX = 0, testErr = 0;    
uint8_t dataGet =0;
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Buffer used for transmission */
uint8_t aTxStartMessage[] = "\n\r Tx Start Message \n\r";
uint8_t aTxEndMessage[] = "\n\r Tx Finish Message \n\r";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Transfer in transmission process is correct */
   __no_operation();
   testTX++;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /*Transfer in reception process is correct */
   dataGet = 1;
   testRX++;
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Transfer error in reception/transmission process */
  __no_operation();
  testErr++;
}

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  uart initialisation
  * @param  boudRate integer baud rate
  * @retval None
  */
void uartInit(uint32_t baudRate)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = ODD parity
      
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance          = USART1;
  
  UartHandle.Init.BaudRate     = baudRate;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /*  in case of Initialization Error */
    _Error_Handler(__FILE__, __LINE__);
  }
}
/**
  * @brief  uart start RX
  *     Put UART peripheral in reception process  
  *     Any data received will be stored "aRxBuffer" buffer : the number max of 
  *     data received is RXBUFFERSIZE
  * @param  None
  * @retval None
  */
void uartStartRX(void)
{
  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY);
  dataGet =0;
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /*Transfer error in reception process */
   _Error_Handler(__FILE__, __LINE__);
  }
}


/**
  * @brief  check if data received from UART
  * @param  None
  * @retval 1 - data received 0 - no data
  */
uint8_t uartIsData(void)
{
  if(dataGet == 1)
  {
        /*Send the received Buffer ###########################################*/  
      if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aRxBuffer, RXBUFFERSIZE)!= HAL_OK)
      {
        /* Transfer error in transmission process */
       _Error_Handler(__FILE__, __LINE__);
      }
      
      /*Wait for the end of the transfer ###################################*/  
      while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
      {
      }
        return 1;
  }
  else return 0;
}
/**
  * @brief  get data received from UART
  * @param  None
  * @retval data received
  */
char uartGetData(void)
{
  return aRxBuffer[0];
}
/**
  * @}
  */ 