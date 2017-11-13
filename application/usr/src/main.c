//******************************************************************************
//*
//*           Copyright (c) 2015 xx-xx-xx Corporation
//*                  ALL RIGHTS RESERVED
//*
//******************************************************************************

//******************************************************************************
//*  $Id: main.c 1686 2017-11-25 10:14:45Z  $
//*
//*Description:
//*******************************************************************************

#include "init.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "task.h"
#include "kernel.h"
#include "superTask.h"
#include "ManageChargerTask.h"
/************************************************
************************************************/


int main(void)
{
    u8 len;	
		u16 times=0; 

		int cnt;
		////////////////////////////
		DINT;
		////////////////////////////
		///////////////////////////////
		STM32_Initial();
	   ///////////////////////
		OSInit();
	   ////////////////////////////
		OSTaskCreate(sSuperTask,300,eSuperPrio);  //0
		OSTimerStart(eSuperPrio,0,20);
	
		OSTaskCreate(sManageChargerTask,300,eManageChargerPrio); //1
		OSTimerStart(eManageChargerPrio,0,20);	
	

		OSTaskCreate(sCpiFSMTask,300,eCpiFSMPrio);  //2
		OSTimerStart(eCpiFSMPrio,0,10);	
		 
		OSTaskCreate(sHmiTask,300,eHmiPrio);   //3
		OSTimerStart(eHmiPrio,0,50);
		
		OSTaskCreate(sFileTask,300,eFilePrio);  //4
		OSTimerStart(eFilePrio,0,100);
		
		OSTaskCreate(sDebugConsoleCommTask,300,eDebugConsoleCommPrio);  //5
		OSTimerStart(eDebugConsoleCommPrio,0,50);
		
		OSTaskCreate(sLogTask,300,eLogPrio);  //6
		OSTimerStart(eLogPrio,0,200);
		

		OSTaskCreate(sSDDiskTask,300,eSDDiskPrio);  //7
		OSTimerStart(eSDDiskPrio,0,50);

		OSTaskCreate(sUSBTask,300,eUSBPrio);  //8
		OSTimerStart(eUSBPrio,0,50);

		OSTaskCreate(sEthernetTask,300,eEthernetPrio);//9
		OSTimerStart(eEthernetPrio,0,200);


	  EINT;
		
		OSStart();
		////////////////////////////////
    while(1)
    {
			/*
				if(USART_RX_STA&0x8000)
				{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				printf("\r\n�����͵���ϢΪ:\r\n");
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
				}else
				{
				times++;
				if(times%5000==0)
				{
				printf("\r\nALIENTEK ������STM32F429������ ����ʵ��\r\n");
				printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
				}
				if(times%200==0)printf("����������,�Իس�������\r\n");  
				if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
				delay_ms(10);   
				} 
				*/
				
					cnt++;
					if(cnt == 10000)
					{
						//LED0 = 1;
					}
					else if (cnt == 50000)
					{
						//LED0 = 0;
						//////////////////////////////////////////////////////////
						///     UART ----------------- ----------------------/////
						//////////////////////////////////////////////////////////
						/*
						if(USART_RX_STA&0x8000)
						{					   
							len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
							printf("\r\n�����͵���ϢΪ:\r\n");
							HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
							while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
							printf("\r\n\r\n");//���뻻��
							USART_RX_STA=0;
						}
						else
						{
							times++;
							if(times%5000==0)
							{
							printf("\r\nALIENTEK ������STM32F429������ ����ʵ��\r\n");
							printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
							}
							if(times%200==0)printf("����������,�Իس�������\r\n");  
							if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
							delay_ms(10);   
						}
						*/
						//////////////////////////////////////////////////////////
						///     UART ----------------- ----------------------/////
						//////////////////////////////////////////////////////////
					}
					else if(cnt == 10000000)
					{
						cnt = 0;
						LED0=!LED0;
					}
    } 
}

