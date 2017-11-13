//******************************************************************************
//*
//*           Copyright (c) 2017 xx-xx-xx Corporation
//*                  ALL RIGHTS RESERVED
//*
//******************************************************************************

//******************************************************************************
//*  $Id: superTask.c 1686 2017-03-25 10:14:45Z  $
//*
//*Description:
//*******************************************************************************
#include "superTask.h"
#include "task.h"
#include "led.h"

typedef struct
{
    int16_t degree;
    uint16_t adcVal;
}ntc_t;




//////////////////////////////////////////////////////////////////////////////
//Function Name  :
//Description    :主任务
//Input          : None
//Output         : None.
//Return         : None.
//////////////////////////////////////////////////////////////////////////////
uint16_t test = 0;
void sSuperTask(void)
{
    TASK_EVENT event;
		int i = 100;
    int j = 0;
	  static int cntSuper= 0;
	  static float ftest1 = 0.0;
    while (1)
    {
        event = OSMaskEventPend(ANYEVENT);

        if (checkEventID(event, eSuperTimer))
        {
					i++;
					i++;
					i++;
					if(i>=30)
					{
						//LED0=!LED0;
						i=0;
					}
        }
				ftest1 += 0.01;
				OSTimeDly(1000);
				//printf("\r\n----在superTask任务 第 %d 次!!!\r\n",cntSuper);
				//printf("\r\n----在superTask任务 技术float = %f 次!!!\r\n",ftest1);
				LED0=!LED0;
				j++;
				j++;
				cntSuper++;
				
    }
}



