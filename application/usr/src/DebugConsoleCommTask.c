//******************************************************************************
//*
//*           Copyright (c) 2015 xx-xx-xx Corporation
//*                  ALL RIGHTS RESERVED
//*
//******************************************************************************

//******************************************************************************
//*  $Id: superTask.c 1686 2015-03-25 10:14:45Z  $
//*
//*Description:
//*******************************************************************************
#include "DebugConsoleCommTask.h"
#include "task.h"
#include "led.h"


INT8U array_ubTraceBank[5][6];
struct stTraceBank_address
{
	INT8U *pbTraceBank_start;
	INT8U *pbTraceBank_NowRead;	
	INT8U *pbTraceBank_NowWrite;
	INT16U uwTraceRead_offset;
	INT16U uwTraceWrite_offset;	
	
};

struct stTraceBank_address stTrace;

//stTrace.pbTraceBank_start = array_ubTraceBank;

//////////////////////////////////////////////////////////////////////////////
//Function Name  :
//Description    :主任务
//Input          : None
//Output         : None.
//Return         : None.
//////////////////////////////////////////////////////////////////////////////

void sDebugConsoleCommTask(void)
{
    TASK_EVENT event;
		int i = 100;

    while (1)
    {
        event = OSMaskEventPend(ANYEVENT);

        if (checkEventID(event, eDebugConsoleCommTimer))
        {
					i++;
					i++;
					i++;
					if(i>=30)
					{
						//LED0=!LED0;
						i=0;
						/////////////////////
						array_ubTraceBank[1][2]=10;
					  INT8U **pbuf = (INT8U **)array_ubTraceBank;  ///这个不是二维指针，是二级指针，指向指针的指针
						INT8U (*pbuf1)[6] ;     ///二维数组的指针
						pbuf1 = array_ubTraceBank;
						INT8U test2 = pbuf1[1][2];
						printf("\r\n----在debug_console任务  >>>>>>>> test2 = %d 次!!!\r\n",test2);
						
						INT8U test = *(*(array_ubTraceBank+1)+2);
						printf("\r\n----在debug_console任务 -1 技test = %d 次!!!\r\n",test);
						
						test = *(*(pbuf+1)+2);
						printf("\r\n----在debug_console任务 -2 技test = %d 次!!!\r\n",test);
						
						test = pbuf[1][2];
						///////////////////////
						printf("\r\n----在debug_console任务 -3 技test = %d 次!!!\r\n",test);
						
					}
        }
    }
}



