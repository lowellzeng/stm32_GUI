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
#include "ManageChargerTask.h"
#include "task.h"
#include "led.h"
#include "can_to_spi_MCP2515.h" 

//////////////////////////////////////////////////////////////////////////////
//Function Name  :
//Description    :Ö÷ÈÎÎñ
//Input          : None
//Output         : None.
//Return         : None.
//////////////////////////////////////////////////////////////////////////////

void sManageChargerTask(void)
{
    TASK_EVENT event;
		int i = 100;

    while (1)
    {
        event = OSMaskEventPend(ANYEVENT);

        if (checkEventID(event, eManageChargerTimer))
        {
					i++;
					i++;
					i++;
					if(i>=30)
					{
						//LED0=!LED0;
						i=0;
						//SPI_Send(0xAD);
						SPI2_ReadWriteByte(0Xad);
						//SPI5_ReadWriteByte(0Xad);
					}
        }
    }
}



