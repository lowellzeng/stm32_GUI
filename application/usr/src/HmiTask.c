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
#include "HmiTask.h"
#include "task.h"
#include "led.h"




//////////////////////////////////////////////////////////////////////////////
//Function Name  :
//Description    :������
//Input          : None
//Output         : None.
//Return         : None.
//////////////////////////////////////////////////////////////////////////////

void sHmiTask(void)
{
    TASK_EVENT event;
		int i = 100;
    static int cntSuper =0; 
	  static float ftest1= 0.0;
    while (1)
    {
        event = OSMaskEventPend(ANYEVENT);

        if (checkEventID(event, eHmiTimer))
        {
					i++;
					i++;
					i++;
					if(i>=200)
					{
						//LED0=!LED0;
						printf("\r\n >>> : -��HmiTask ���� �� %d ��!!!\r\n",cntSuper);
						printf("\r\n >>> : -��HmiTask ����float = %f ��!!!\r\n",ftest1);
						ftest1 += 0.05;
						cntSuper++;
						i=0;
					}
        }
    }
}



