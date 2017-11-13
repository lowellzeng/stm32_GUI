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
#include "CpiFSMTask.h"
#include "task.h"
#include "led.h"
#include "can.h"


///////////////////////////////////////////

uint8_t bArrayRxMsgQueue[8];
uint8_t bArrayTxMsgQueue[8];
//////////////////////////////////////////////////////////////////////////////
//Function Name  :
//Description    :主任务
//Input          : None
//Output         : None.
//Return         : None.
//////////////////////////////////////////////////////////////////////////////

void sCpiFSMTask(void)
{
    TASK_EVENT event;
		static int i = 0;
	  static uint16_t uwSendLoop = 0;
		uint16_t uwCnt = 0;
	  uint8_t  res = 0;
	
    while (1)
    {
        event = OSMaskEventPend(ANYEVENT);

        if (checkEventID(event, eCpiFSMTimer))
        {
					i +=3;
					
					if(i>=100)
					{
						uwSendLoop++;
						for(uwCnt=0;uwCnt<8;uwCnt++)
						{
							bArrayTxMsgQueue[uwCnt]=uwSendLoop+uwCnt;//填充发送缓冲区
							
						}
						res=CAN1_Send_Msg(bArrayTxMsgQueue,8);//发送8个字节
						i=0;
					}
        }
    }
}



