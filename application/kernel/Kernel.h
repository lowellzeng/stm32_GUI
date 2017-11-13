/************************************************************************************************
xxxxxxx ELECTRONIC Ltd										*
RTOS kernel for xxxxx ,All rights reserved							*
VERSION:		V00									*
File name:		OS_HEAD.H								*
Description:		DEFINITION OF DATA STRUCTURE AND GLOBAL VARIABLES			*
Update Date:		2003/07/22								*
Writer:			xxxxxx									*
Modifier:		xxxxx									*
*************************************************************************************************/

#ifndef	_Kernel_H
#define _Kernel_H				//防止重复包含

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

#include "ports\OS_CPU.h"
#include "ports\os_config.h"
#include "stdint.h"
#include "stm32f4xx.h"

/*
*************************************************************************************************
*                                          task ready control					*
*												*
* Description: 	These two macros control the ready bit of a task in OSRdyMap			*											*
* 		OSTaskRdy insert a ready task into the OSRdyMap 				*
*		OSTaskSuspend	delete a redy task from OSRdyMap				*
*************************************************************************************************
*/
//新加入延时函数
#define BIT(x) (1LL << (x))
#define ANYEVENT (~0)
#define EVENT(id) BIT(id)
#define checkEventID(event, id) ((event) & EVENT(id))
#define checkEventIDExt(event,eventMask)   (((event) & (eventMask))==(eventMask))
#define checkEvent(event,eventMask) ((event) & (eventMask))
#define msToTicks(ms) ((uint32_t)(ms))
#define knlIsTimeOut(time) knlChkTimeOut(&time)



#define	OSTaskRdy(prio)			OSRdyMap|=(1<<(prio))
#define OSTaskSuspend(prio)		OSRdyMap&=~(1<<(prio))


/************************************************************************************************/
/*				constant for stack check					*/
/************************************************************************************************/
#if	OS_STK_CHK_EN>0
#define	OS_STK_FULL	0
#endif

/************************************************************************************************/
/*				macro for CPU USEAGE CALCULATION				*/
/************************************************************************************************/

#if	OS_TASK_IDLE_EN>0
#define	OSGetCPUTimeFree()		OSCpuFree
#endif

/************************************************************************************************/
/*				macro for timer							*/
/************************************************************************************************/

#define OSTimerStart(prio,cnt,period)	OSTCBTbl[(prio)].TimerCnt=(cnt); OSTCBTbl[(prio)].TimerPeriod=(period)
#define OSTimerStop(prio)		OSTCBTbl[(prio)].TimerPeriod=0
		
/************************************************************************************************/
/*				macro for event							*/
/************************************************************************************************/

#define	OSEventPend()			OSMaskEventPend(0)

/*
*************************************************************************************************
*                                        SYSTEM EVENT DEFINE					*
*************************************************************************************************/

#define 	OS_EVENTID_TIMER		0

/*
*************************************************************************************************
*                                        CONSTANT FOR TIMER					*
*************************************************************************************************/
#define false			 		0
#define true					1


#define  OS_FALSE                  0
#define  OS_TRUE                   1

typedef uint8_t  bool_t;

/*
*************************************************************************************************
*                                       TASK CONTROL BLOCK 					*
*************************************************************************************************/

typedef struct{
		OS_STK	*OSTCBStkPtr; 		/*		Pointer to current top of stack	*/
    
		#if	OS_STK_CHK_EN>0
		OS_STK	*OSTCBStkBot;		/*		Pointer to bottom of stack	*/
		
		INTOS	OSTCBStkSize;		/*		stack size			*/
		#endif
		
		INT32U	TimerPeriod;		/*		timer period			*/
		
		INT32U	TimerCnt;		/*		timer counter			*/
		
		TASK_EVENT	OSEvent;	/*		event 				*/
		
		TASK_EVENT	OSEventBitMask;	/*		event mask bits			*/
	
	  INT32U       OSTCBDly;   //lowell add
		}OS_TCB;
		

/*
*************************************************************************************************
*                                        TASK SCHED VARIABLES DEFINE				*
*************************************************************************************************/
OS_EXT  INT16U	OSRdyMap;

OS_EXT  OS_TCB	OSTCBTbl[OS_LOWEST_PRIO + 1];	/* Table of TCB                           	*/

OS_EXT  INTOS   OSPrioHighRdy;	         	/* Priority of highest priority in ready table  */
OS_EXT  INTOS   OSPrioCur;	            	/* Priority of current task  			*/
OS_EXT  OS_TCB	*OSTCBHighRdy;			/* Pointer to highest priority TCB R-to-R 	*/
OS_EXT  OS_TCB  *OSTCBCur;			/* Pointer to currently running TCB  		*/

//OS_EXT 	OS_STK  OSSystemStk[OS_MAX_STK_SIZE];   /* Stack of SYSTEM task				*/


OS_EXT	OS_STK	*OSStkPtr;

OS_EXT	BOOLEAN  OSRunning;

OS_EXT  INT8U    OSIntNesting;             /* Interrupt nesting level */

/*
*************************************************************************************************
*                                      CPU USAGE 						*
*************************************************************************************************/

#if	OS_TASK_IDLE_EN>0

OS_EXT  INT32U  	OSIdleCtr;
OS_EXT	INT32S/*INT16U*/	OSTime;     //////////////////////////////////////////////
OS_EXT	INTOS		OSCpuFree;

#endif

/*
*************************************************************************************************
*                                      PRIORITY RESOLUTION TABLE 						*
*************************************************************************************************/

#if	OS_TABLE_IN_FLASH>0
extern 	INTOS	const  OSUnMapTbl[16];
#else
extern 	 INTOS  OSUnMapTbl[16];
#endif

void OSInitHookBegin(void);
void OSSched (void);
void  OSInitSys (void);
void 	OSInitRdyList(void);   
void	OSInitTCBList(void);
INTOS	OSFindHighPrio(void);
void	OSStartHighRdy(void);
OS_STK *OSTaskStkInit(  void (*task)(void),  void  *parg,OS_STK *ptos);
void	OSISREventSend(INTOS prio,INTOS EventId);

#if	OS_TASK_IDLE_EN>0
void  OSInitTaskIdle (void);
#endif


/*
*************************************************************************************************
*                                        SYSTEM FUNCTION LIST					*
*************************************************************************************************
*/
void		OSInit (void);

void		OSStart (void);

void  		OSTaskCreate (void (*task)(void), INTOS stksize, INTOS prio);

TASK_EVENT	OSMaskEventPend(TASK_EVENT mask);

TASK_EVENT	OSEventGet(void);

void		OSEventSend(INTOS prio,INTOS EventId);

void  OSIntEnter (void);
void  OSIntExit (void);

void	OSTimerTick(void);
void  OSTimeDly (INT32U ticks);

#if		OS_STK_CHK_EN>0
INTOS		OSStkChk(INTOS	prio);
#endif
//添加定时器
int32_t knlGetTargetTime(uint32_t msTime);
bool knlChkTimeOut(int32_t* targetTime);


#endif














