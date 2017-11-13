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
//Description    :������
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
					  INT8U **pbuf = (INT8U **)array_ubTraceBank;  ///������Ƕ�άָ�룬�Ƕ���ָ�룬ָ��ָ���ָ��
						INT8U (*pbuf1)[6] ;     ///��ά�����ָ��
						pbuf1 = array_ubTraceBank;
						INT8U test2 = pbuf1[1][2];
						printf("\r\n----��debug_console����  >>>>>>>> test2 = %d ��!!!\r\n",test2);
						
						INT8U test = *(*(array_ubTraceBank+1)+2);
						printf("\r\n----��debug_console���� -1 ��test = %d ��!!!\r\n",test);
						
						test = *(*(pbuf+1)+2);
						printf("\r\n----��debug_console���� -2 ��test = %d ��!!!\r\n",test);
						
						test = pbuf[1][2];
						///////////////////////
						printf("\r\n----��debug_console���� -3 ��test = %d ��!!!\r\n",test);
						
					}
        }
    }
}



