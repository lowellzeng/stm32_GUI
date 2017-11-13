/************************************************************************************************
xxxxxxxxxxxxxxxxxxxxxxxxxxx Ltd										*
RTOS kernel for electronics ,All rights reserved							*
VERSION:		V00									*
File name:		OS_SHELL.C								*
Description:		TASK SCHEDULE FUNCTIONS							*
Update Date:		2015/06/10								*
Writer:			xxxxxxx									*
*************************************************************************************************/
#define	 OS_GLOBALS

#include "Kernel.h"

extern void OSIntCtxSw(void);


/**************************************************************************************************
*                                             INITIALIZATION					*
*												*
* Description: This function is used to initialize the internals of RTOS and MUST be called 	*
*              prior to creating any RTOS object and, prior to calling OSStart().		*
*												*
* Arguments  : none										*
*												*
* Returns    : none										*
**************************************************************************************************/

void  OSInit (void)
{
 OSInitHookBegin();
	
	OSInitSys();                             

 	OSInitRdyList();                          
    
	OSInitTCBList();                            
   
#if	OS_TASK_IDLE_EN>0
	OSInitTaskIdle();        
#endif                   

}

/*
*************************************************************************************************
*                                          START MULTITASKING					*
*												*
* Description: This function is used to start the multitasking process which lets kernal	* 
*              manages the task that you have created.  Before you can call OSStart(), you 	*
*              MUST have called OSInit() and you MUST have created at least one task.		*
*												*
* Arguments  : none										*
*												*
* Returns    : none										*
*												*
*************************************************************************************************
*/

void  OSStart (void)
{
    OSPrioHighRdy = OSFindHighPrio();                              /* Find highest priority's task priority number   */
    OSPrioCur     = OSPrioHighRdy;
		OSTCBHighRdy  = &OSTCBTbl[OSPrioHighRdy]; /* Point to highest pri task ready to run	*/
		OSTCBCur      = OSTCBHighRdy;
		OSStartHighRdy();                         /* Execute target  code to start task      	*/
}




/* 
*************************************************************************************************
*                                             Create a task 					*
* Description: 	This function is used to have RTOS manage the execution of a task.  Tasks can 	*
*              	either be created prior to the start of multitasking or by a running task.  A 	*
*              	task cannot be created by an ISR.						*
*												*
* Arguments  : 	task    is a pointer to the task's code						*
*              	ptos    is a pointer to the task's top of stack. 				*
*              	prio    is the task's priority.  A unique priority MUST be assigned to each task* 
*                       and the lower the number, the higher the priority.			*
*		pq	pointer to a message queue related with task				*												*
*												*
* Returns    : 	none										*
*************************************************************************************************
*/
void  	OSTaskCreate (void (*task)(void), INTOS stksize, INTOS prio)
{
	OS_TCB	*ptcb;
	OS_STK	*psp;
	OS_STK  *ptos;

	
	ptos=OSStkPtr;
	
#if	OS_STK_CHK_EN>0
	OSTCBTbl[prio].OSTCBStkBot=ptos;
	OSTCBTbl[prio].OSTCBStkSize=stksize;
#endif

#if	OS_STK_GROWTH_UP>0
	OSStkPtr+=stksize;
#else
	OSStkPtr -= stksize;
#endif
	//原版山特只有两个参数，这里必须3个，其中2没有用处，只是为了凑3个参数
	psp = (void*)OSTaskStkInit(task, (void *) 2, ptos);		
	
	ptcb=&OSTCBTbl[prio];
	
	ptcb->OSTCBStkPtr=psp;
	
	ptcb->TimerPeriod=0;
	
	ptcb->TimerCnt=0;
	
	ptcb->OSEvent=0;
	
	OSTaskRdy(prio);
	
}

void  OSIntEnter (void)
{
	OS_ENTER_CRITICAL();
		if (OSIntNesting < 255) 
		{
			OSIntNesting++;                      /* Increment ISR nesting level                        */
		}
	OS_EXIT_CRITICAL();

}

/*
*************************************************************************************************
*                                               EXIT ISR					*
*												*
* Description: When the last nested ISR has completed, OS will call the scheduler to determine	* 
*              whether a new, high-priority task, is ready to run.				*
*												*
* Arguments  : none										*
*												*
* Returns    : none										*
*************************************************************************************************
*/

void  OSIntExit (void)
{
        OS_ENTER_CRITICAL();
        if (OSIntNesting > 0) {                            /* Prevent OSIntNesting from wrapping       */
            OSIntNesting--;
        }
        if (OSIntNesting == 0) {                           /* Reschedule only if all ISRs complete ... */
        	OSPrioHighRdy =OSFindHighPrio();
        	if (OSPrioHighRdy != OSPrioCur) 
        	{              					/* No Sw if current task is highest rdy*/
        		OSPrioCur=OSPrioHighRdy;	//山特有，网上原版没有,都可以，因为OSIntCtxSw已经对OSPrioCur和OSPrioHighRdy处理了
                OSTCBHighRdy=&OSTCBTbl[OSPrioHighRdy];
                OSIntCtxSw();                           /* Perform interrupt level ctx switch  */
        	}
        }
        OS_EXIT_CRITICAL();
}

/*
*************************************************************************************************
*                                               Stack Check					*
*												*
* Description: Calculate the stack usage of task						*												*
* Arguments  : prio:		the ID of task							*
* Returns    : if the Left stack size is less than 10,return OS_STK_FULL			*
*	       else return the used stack number						*
*************************************************************************************************
*/
#if	OS_STK_CHK_EN>0
INTOS	OSStkChk(INTOS	prio)
{
	OS_STK	*stk;
	INTOS	i;
	i=0;
	
	stk=OSTCBTbl[prio].OSTCBStkBot;
	
	while(1)
	{
		if((*stk)==0)
		{
			i++;
			if(i==10)
			{
				#if	OS_STK_GROWTH_UP>0
				return(stk-OSTCBTbl[prio].OSTCBStkBot-10);
				#else
				return(OSTCBTbl[prio].OSTCBStkBot-stk-10);
				#endif
			}
		}
		else
		{
			i=0;
		}
		#if	OS_STK_GROWTH_UP>0
		stk++;
		if(stk==OSTCBTbl[prio].OSTCBStkBot+OSTCBTbl[prio].OSTCBStkSize)
		{
			return(OS_STK_FULL);
		}
		#else
		stk--;
		if(stk==OSTCBTbl[prio].OSTCBStkBot-OSTCBTbl[prio].OSTCBStkSize)
		{
			return(OS_STK_FULL);
		}
		#endif
	}
}
#endif
			
		

