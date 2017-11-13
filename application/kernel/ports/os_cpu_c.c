;/*********************** (C) COPYRIGHT 2015  *************************
;* File Name	: os_cpu_c.c 
;* Author		: xxxxxxxxx
;* Version		: V1.0
;* Date			: 06/10/2015
;* Description	: ¦ÌCOS-II asm port	for STM32F407
;*******************************************************************************/
#define  OS_CPU_GLOBALS

#include	"OS_CPU.h"
#include "kernel.h"

OS_STK  *OS_CPU_ExceptStkBase;
OS_STK   OS_CPU_ExceptStk[128];


void  OS_TaskReturn (void)
{

	for (;;) {
	}

}
////////////////////////////////////////////////////
OS_STK *OSTaskStkInit (void (*task)(void), void *p_arg, OS_STK *ptos)
{
    OS_STK *p_stk;


                                                  /* 'opt' is not used, prevent warning                   */
    p_stk      = ptos + 1u;                                     /* Load stack pointer                                   */
                                                                /* Align the stack to 8-bytes.                          */
    p_stk      = (OS_STK *)((OS_STK)(p_stk) & 0xFFFFFFF8u);
                                                                /* Registers stacked as if auto-saved on exception      */
    *(--p_stk) = (OS_STK)0x01000000uL;                          /* xPSR                                                 */
    *(--p_stk) = (OS_STK)task;                                  /* Entry Point                                          */
    *(--p_stk) = (OS_STK)OS_TaskReturn;                         /* R14 (LR)                                             */
    *(--p_stk) = (OS_STK)0x12121212uL;                          /* R12                                                  */
    *(--p_stk) = (OS_STK)0x03030303uL;                          /* R3                                                   */
    *(--p_stk) = (OS_STK)0x02020202uL;                          /* R2                                                   */
    *(--p_stk) = (OS_STK)0x01010101uL;                          /* R1                                                   */
    *(--p_stk) = (OS_STK)p_arg;                                 /* R0 : argument                                        */

                                                                /* Remaining registers saved on process stack           */
    *(--p_stk) = (OS_STK)0x11111111uL;                          /* R11                                                  */
    *(--p_stk) = (OS_STK)0x10101010uL;                          /* R10                                                  */
    *(--p_stk) = (OS_STK)0x09090909uL;                          /* R9                                                   */
    *(--p_stk) = (OS_STK)0x08080808uL;                          /* R8                                                   */
    *(--p_stk) = (OS_STK)0x07070707uL;                          /* R7                                                   */
    *(--p_stk) = (OS_STK)0x06060606uL;                          /* R6                                                   */
    *(--p_stk) = (OS_STK)0x05050505uL;                          /* R5                                                   */
    *(--p_stk) = (OS_STK)0x04040404uL;                          /* R4                                                   */



    return (p_stk);
}


/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void  OS_CPU_FP_Reg_Push     (OS_STK   *stkPtr);
void  OS_CPU_FP_Reg_Pop      (OS_STK   *stkPtr);



/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

/////////////////////////////////////////////////////////////////////
///
///
/////////////////////////////////////////////////////////////////////


void  OSInitHookBegin (void)
{
    INT32U   size;
    OS_STK  *pstk;

                                                                /* Clear exception stack for stack checking.            */
    pstk = &OS_CPU_ExceptStk[0];
    size = 128;
    while (size > 0u) {
        size--;
       *pstk++ = (OS_STK)0;
    }

                                                                /* Align the ISR stack to 8-bytes                       */
    OS_CPU_ExceptStkBase = (OS_STK *)&OS_CPU_ExceptStk[128];
    OS_CPU_ExceptStkBase = (OS_STK *)((OS_STK)(OS_CPU_ExceptStkBase) & 0xFFFFFFF8);

}



