/*******************************************************************************
  * 文件：Typedefine.c
  * 作者：https://blog.csdn.net/wanglong3713
  * 版本：v1.0
  * 日期：2020-10-20
  * 说明：类型定义，通用
*******************************************************************************/
#ifndef _TYPEDEFINE_H_
#define _TYPEDEFINE_H_

#include <stdint.h>

/*
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef signed short        int16_t;
typedef unsigned short      uint16_t;
typedef signed long         int32_t;
typedef unsigned long       uint32_t;
*/
typedef unsigned char       bool;

typedef union
{
	struct
	{
		uint8_t b0:1;
		uint8_t b1:1;
		uint8_t b2:1;
		uint8_t b3:1;
		uint8_t b4:1;
		uint8_t b5:1;
		uint8_t b6:1;
		uint8_t b7:1;
	}bt;
	uint8_t byte;
}Byte_tu;//单字节类型定义

typedef union
{
	struct
	{
		uint8_t b0 :1;
		uint8_t b1 :1;
		uint8_t b2 :1;
		uint8_t b3 :1;
		uint8_t b4 :1;
		uint8_t b5 :1;
		uint8_t b6 :1;
		uint8_t b7 :1;
		uint8_t b8 :1;
		uint8_t b9 :1;
		uint8_t b10:1;
		uint8_t b11:1;
		uint8_t b12:1;
		uint8_t b13:1;
		uint8_t b14:1;
		uint8_t b15:1;
	}bt;
	struct
	{
		uint8_t lowb;		  
		uint8_t highb;
	}byte;
	uint16_t word;
}Word_tu;//字（双字节）类型定义

typedef union
{	
	struct
	{
		uint8_t b0 :1;
		uint8_t b1 :1;
		uint8_t b2 :1;
		uint8_t b3 :1;
		uint8_t b4 :1;
		uint8_t b5 :1;
		uint8_t b6 :1;
		uint8_t b7 :1;
		uint8_t b8 :1;
		uint8_t b9 :1;
		uint8_t b10:1;
		uint8_t b11:1;
		uint8_t b12:1;
		uint8_t b13:1;
		uint8_t b14:1;
		uint8_t b15:1;
		uint8_t b16:1;
		uint8_t b17:1;
		uint8_t b18:1;
		uint8_t b19:1;
		uint8_t b20:1;
		uint8_t b21:1;
		uint8_t b22:1;
		uint8_t b23:1;
		uint8_t b24:1;
		uint8_t b25:1;
		uint8_t b26:1;
		uint8_t b27:1;
		uint8_t b28:1;
		uint8_t b29:1;
		uint8_t b30:1;
		uint8_t b31:1;
   }bt;
   struct
   {
		uint8_t lowb; 			  
		uint8_t mid1b;
		uint8_t mid2b;			   
		uint8_t highb;
	}byte;		   
	struct
	{
		uint16_t loww;						   
		uint16_t highw;								 
	}word;				 
	uint32_t dword;	
}DWord_tu;//双字（四字节）类型定义

#define SetBit(VAR,Place)         ( VAR |= (1<<Place) )
#define ClrBit(VAR,Place)         ( VAR &= ((1<<Place)^255) )
#define ChgBit(VAR,Place)         ( VAR ^= (1<<Place) )
#define ValBit(VAR,Place)         (VAR & (1<<Place))

#endif
/*************************END OF FILE**********************/