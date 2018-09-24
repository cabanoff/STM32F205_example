/*	
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* this code needs standard functions memcpy() and memset()
   and input/output functions _inbyte() and _outbyte().

   the prototypes of the input/output functions are:
     int _inbyte(unsigned short timeout); // msec timeout
     void _outbyte(int c);

 */

//#include "crc16.h"
#include "xmodem.h"
//#include <string.h>
#include "intrinsics.h"


#define DLY_1S (1000)
#define MAXRETRANS 25

// msec timeout
int default_inbyte(unsigned short timeout);
void default_outbyte(int c);

typedef int(*_inbyte_t)(unsigned short timeout);
typedef void(*_outbyte_t)(int c);

_inbyte_t _inbyte = default_inbyte;
_outbyte_t _outbyte = default_outbyte;

int default_inbyte(unsigned short timeout)
{
  if(timeout == 0)return -1;
  else return 0;
}
void default_outbyte(int c)
{
  if(c > 0)__no_operation();
}
// assign in and out functions
void xmodenInit(int(*inbyte)(unsigned short), void(*outbyte)(int))
{
  _inbyte = inbyte;
  _outbyte = outbyte;
}

unsigned short crc16_ccitt(const unsigned char *buf, int sz)
{
	unsigned short crc = 0;
	while (--sz >= 0) 
        {
		int i;
		crc ^= (unsigned short) *buf++ << 8;
		for (i = 0; i < 8; i++)
                {
			if (crc & 0x8000)crc = crc << 1 ^ 0x1021;
			else crc <<= 1;
		
                }
	}
	return crc;
}

int check(int crc, const unsigned char *buf, int sz)
{
	if (crc) {
		unsigned short _crc = crc16_ccitt(buf, sz);
		unsigned short _tcrc = (buf[sz]<<8)+buf[sz+1];
		if (_crc == _tcrc)
			return 1;
	}
	else {
		int i;
		unsigned char cks = 0;
		for (i = 0; i < sz; ++i) {
			cks += buf[i];
		}
		if (cks == buf[sz])
		return 1;
	}

	return 0;
}

char debugInData[0x100];
char debugPointer = 0;
char debugInFlushData[0x100];
char debugFlushPointer = 0;

static int flushinput(int value)
{
        int debugFlush;
	//while (_inbyte(((DLY_1S)*3)>>1) >= 0)
        while(1)
        {
          debugFlush = _inbyte(((DLY_1S)*3)>>1);
          if(debugFlush < 0)return value;
          debugInFlushData[debugFlushPointer] = (char)debugFlush;
          debugFlushPointer++;
        }
        //return value;		
}

static int convStr2Byte(unsigned char *str2Byte)
{
  int result = 0;
  if((str2Byte[2] != 0x20)&&(str2Byte[2] != 0x0d))return -1;
  if((str2Byte[0] >= ('0'))&&(str2Byte[0] <= ('9')))result = (str2Byte[0]-'0')<<4;
  else if((str2Byte[0] >= ('A'))&&(str2Byte[0] <= ('F')))result = (str2Byte[0]-'A'+10)<<4;
  else return -1;
  if((str2Byte[1] >= ('0'))&&(str2Byte[1] <= ('9')))result += (str2Byte[1]-'0');
  else if((str2Byte[1] >= ('A'))&&(str2Byte[0] <= ('F')))result += (str2Byte[1]-'A'+10);
  else return -1;
  return result;
}



int xmodemReceive(unsigned char *dest, int destsz)
{
  unsigned char header[] = {"@08010000"};
  unsigned char str2Byte[3];
  int inData = -1;
  int pointer = 0;
  if(destsz == 0)return -2;
  for(int i = 20; i > 0; i--) //delay 20sec
  {
    inData = _inbyte(DLY_1S);
    if(inData >= 0)break;
  }
  if(inData < 0)return -1;
  /*
   * receive header @08010000\n
  */
  for(unsigned int i =0; i < sizeof(header)-1;i++)
  {
    if(inData != header[i])return flushinput(-2);
    inData = _inbyte(DLY_1S);
  }
  if(inData != 0x0d)return flushinput(-2);
  
  /*
   * receive body
  */
  while(pointer < destsz)
  {
    for(int i = 0; i < 3; i++)
    {
      inData = _inbyte(DLY_1S*2);
      if(inData < 0)return flushinput(-3);      
      str2Byte[i] = inData;
      debugInData[debugPointer]=inData;
      debugPointer++;
      
    }
    inData = convStr2Byte(str2Byte);
    if(inData >= 0)dest[pointer++] = inData;
    else return flushinput(-4);   
  }
  inData = _inbyte(DLY_1S*2);  //receive end of file
  if(inData < 0)return flushinput(-3); 
  if(inData == 'q')return flushinput(pointer); // file received
  return flushinput(-5);  // file too big
}





//int main(void)
//{
//	int st;
//
//	printf ("Send data using the xmodem protocol from your terminal emulator now...\n");
//	/* the following should be changed for your environment:
//	   0x30000 is the download address,
//	   65536 is the maximum size to be written at this address
//	 */
//	st = xmodemReceive((char *)0x30000, 65536);
//	if (st < 0) {
//		printf ("Xmodem receive error: status: %d\n", st);
//	}
//	else  {
//		printf ("Xmodem successfully received %d bytes\n", st);
//	}
//
//	return 0;
//}
