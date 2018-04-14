/**
  ******************************************************************************
  * @file    xmodem.h 
  * @author  AKabanov
  * @brief   Header for xmodem.c module
  ******************************************************************************
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XMODEM_H
#define __XMODEM_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  uart start RX
  *     Put UART peripheral in reception process  
  *     Any data received will be stored "aRxBuffer" buffer : the number max of 
  *     data received is RXBUFFERSIZE
  * @param  dest - buffer to download data
  * @param  destsz - size of buffer to download data
  * @retval number of bytes received; normal end  
  *         -1;  canceled by remote 
  *         -2;  sync error
  *         -3;  too many retry error
  */
int xmodemReceive(unsigned char *, int );

// assign in and out functions
void xmodenInit(int(*inbyte)(unsigned short), void(*outbyte)(int));
//printing test message
void xmodemTest(void);

#endif /* __XMODEM_H */
