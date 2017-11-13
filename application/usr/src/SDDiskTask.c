//******************************************************************************
//*
//*           Copyright (c) 2015 xx-xx-xx Corporation
//*                  ALL RIGHTS RESERVED
//*
//******************************************************************************

//******************************************************************************
//*  $Id: SDDisk.c 1686 2015-03-25 10:14:45Z  $
//*
//*Description:
//*******************************************************************************
#include "SDDisk.h"
#include "task.h"
#include "led.h"




//////////////////////////////////////////////////////////////////////////////
//Function Name  :
//Description    :Ö÷ÈÎÎñ
//Input          : None
//Output         : None.
//Return         : None.
//////////////////////////////////////////////////////////////////////////////

void sSDDiskTask(void)
{
    TASK_EVENT event;
		int i = 100;

    while (1)
    {
        event = OSMaskEventPend(ANYEVENT);

        if (checkEventID(event, eSDDiskTimer))
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
    }
}



