#include "delay.h"
#include "init.h"

#include "os_config.h"
////////////////////////////////////////////////////////////////////////////////// 	 
////////////////////////////////////////////////////////////////////////////////// 

static u32 fac_us=0;							//us��ʱ������
static u16 fac_ms=0;				        //ms��ʱ������,��os��,����ÿ�����ĵ�ms��

			   
//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ��
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(u8 SYSCLK)
{

	u32 reload;

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
	fac_us=SYSCLK;														//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��

	reload=SYSCLK;					    							//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;					//����delay_ostickspersec�趨���ʱ��
																						//reloadΪ24λ�Ĵ���,���ֵ:16777216,��180M��,Լ��0.745s����	
	fac_ms=1000/OS_TICKS_PER_SEC;							//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;	//����SYSTICK�ж�
	SysTick->LOAD=reload; 										//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//����SYSTICK

}								    


////////////////////////////////////////////////////////////////
//��ʱnus
//nusΪҪ��ʱ��us��.	
//nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)	 
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						  //��Ҫ�Ľ����� 
	told=SysTick->VAL;        		  //�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}
//////////////////////////////////////////////////////
//��ʱnms
//nms:Ҫ��ʱ��ms��
///////////////////////////////////////////////////////
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}
			 



































