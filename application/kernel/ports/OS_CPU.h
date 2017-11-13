/************************************************************************************************
XXXXXXX ELECTRONIC Ltd										*
RTOS kernel for xxxx ,All rights reserved							*
VERSION:		V00									*
File name:		OS_CPU.h								*
Description:		DEFINITION OF STRUCTURE RELATED TO CPU					*
Update Date:		2003/07/22								*
Writer:			xxxxx									*
*************************************************************************************************/
#ifndef  OS_CPU_H
#define  OS_CPU_H

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

/****************************************************************************************
*                    Basic variables type definition				*
*                        DATA TYPES
*                     (Compiler Specific)
****************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity       */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity       */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity       */
typedef signed   short INT16S;                   /* Signed   16 bit quantity       */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity       */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity       */
typedef float          FP32;                     /* Single precision floating point*/
typedef double         FP64;                     /* Double precision floating point*/

typedef unsigned short INTOS;				/* the smallest unit int the cpu	*/

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide*/
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register*/
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/************************************************************************************************
*					Macro definitions					*
*************************************************************************************************/
/*
*****************************************************************************************
*                             Cortex M3
*                     Critical Section Management
****************************************************************************************
*/

#define  OS_CRITICAL_METHOD   4

/*
****************************************************************************************
*                          ARM Miscellaneous
****************************************************************************************
*/
#define  OS_TASK_SW()         OSCtxSw()

/*
****************************************************************************************
*                               PROTOTYPES
*                           (see OS_CPU_A.ASM)
****************************************************************************************
*/

#if OS_CRITICAL_METHOD == 4
	 void OS_ENTER_CRITICAL (void);
     void OS_EXIT_CRITICAL (void);     
#endif

void       OSCtxSw(void);
void       OSIntCtxSw(void);
void       OSStartHighRdy(void);

void       OSPendSV(void);

OS_EXT INT32U OSInterrputSum;

#ifndef StkDefine
#define StkDefine
//#pragma DATA_SECTION(OSSystemStk,"OSStack");
//OS_EXT 	OS_STK  OSSystemStk[OS_MAX_STK_SIZE];   /* Stack of SYSTEM task				*/
#endif

#endif

