/************************************************************************************************
xxxxxxxxxxxxxxxxxxxxxxxxxxx Ltd										*
RTOS kernel for electronics ,All rights reserved							*
VERSION:		V00									*
File name:		timer.C								*
Description:		timer tick ISR						*
Update Date:		2015/06/10								*
Writer:			xxxxxxx									*
*************************************************************************************************/
#include "Kernel.h"

#include "stdint.h"
#include "stm32f4xx.h"
///////////////////////
void  OSTimeDly (INT32U ticks)
{
    INT8U      y;


    if (OSIntNesting > 0u) {                    /* See if called with scheduler locked                */
        return;
    }
    if (ticks > 0u) {                            /* 0 means no delay!                                  */
        OS_ENTER_CRITICAL();
        OSTCBCur->OSTCBDly = ticks;              /* Load ticks in TCB                                  */
			
				OSTaskSuspend(OSPrioCur);
        OSSched();                              /* Find next task to run!                             */
			  OS_EXIT_CRITICAL();
    }
}

int32_t testtemp = 0;
///////////////////////////
void	OSTimerTick(void)
{
	OS_TCB	*ptcb;
	OS_TCB	*ptcbend;
	INTOS	prio;
	
#if	OS_TASK_IDLE_EN>0
  OS_ENTER_CRITICAL();     
	OSTime++;
  OS_EXIT_CRITICAL();
  
/*	if(OSTime==OS_TICKS_PER_SEC)  ////////////////////////////////////////
	{
		OSTime=0;
		OSCpuFree=OSIdleCtr*OS_CPUFREE_FACTOR>>20;
		OSIdleCtr=0;
	}*/
#endif 	
	//OS_ENTER_CRITICAL();
		prio=0;
		ptcbend=&OSTCBTbl[OS_LOWEST_PRIO];
		ptcb=OSTCBTbl;
		while(ptcb<=ptcbend)
		{
			if(ptcb->TimerPeriod!=0)
			{
				/*
				ptcb->TimerCnt++;
				
				if(ptcb->TimerCnt>=ptcb->TimerPeriod)
				{
					ptcb->TimerCnt= 0;
				
					{
						OSISREventSend(prio,OS_EVENTID_TIMER);
					}
				
				}
				*/
				//////////////////////
				if (ptcb->OSTCBDly != 0u) 
				{                    // No Delayed or waiting for event with TO 
            ptcb->OSTCBDly--;  
					  testtemp = ptcb->OSTCBDly;
					   testtemp = ptcb->OSTCBDly;
				}
				else if(ptcb->OSTCBDly == 0u)
				{
					OSTaskRdy(prio);
					/////////////
					ptcb->TimerCnt++;
					if(ptcb->TimerCnt>=ptcb->TimerPeriod)
					{
						ptcb->TimerCnt= 0;
						{
							OSISREventSend(prio,OS_EVENTID_TIMER);
						}
					}
					////////////
				 }
				//////////////////////
			}
			ptcb++;
			prio++;
		}
	//OS_EXIT_CRITICAL();
}

//加入定时器
int32_t knlGetTargetTime(uint32_t msTime)
{
    int32_t tmp;
    tmp = OSTime + msToTicks(msTime);
    if(tmp == 0)tmp = 1;
    return tmp;
}

bool knlChkTimeOut(int32_t* targetTime)
{
    bool res;
    if(*targetTime == 0)return true;
    res = (OSTime - *targetTime) >= 0 ? (bool)true : (bool)false;
    if(res)*targetTime = 0;
    return res;
}
