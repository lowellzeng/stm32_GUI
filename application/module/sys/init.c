//******************************************************************************
//*
//*           Copyright (c) 2015 xx-xx-xx Corporation
//*                  ALL RIGHTS RESERVED
//*
//******************************************************************************

//******************************************************************************
//*  $Id: init.c 1686 2017-11-25 10:14:45Z  $
//*
//*Description:
//*******************************************************************************

#include "init.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "can.h"
////////////////////////////////////////////////////////////////////////////////// 

//ʱ��ϵͳ���ú���
//Fvco=Fs*(plln/pllm);
//SYSCLK=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCOƵ��
//SYSCLK:ϵͳʱ��Ƶ��
//Fusb:USB,SDIO,RNG�ȵ�ʱ��Ƶ��
//Fs:PLL����ʱ��Ƶ��,������HSI,HSE��. 
//plln:��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.
//pllm:��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
//pllp:ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
//pllq:USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.

//�ⲿ����Ϊ25M��ʱ��,�Ƽ�ֵ:plln=360,pllm=25,pllp=2,pllq=8.
//�õ�:Fvco=25*(360/25)=360Mhz
//     SYSCLK=360/2=180Mhz
//     Fusb=360/8=45Mhz
//����ֵ:0,�ɹ�;1,ʧ��
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //ʹ��PWRʱ��
    
    //������������������õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���
    //ʱʹ�����빦��ʵ��ƽ�⣬�˹���ֻ��STM32F42xx��STM32F43xx�����У�
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//���õ�ѹ�������ѹ����1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //��HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//��PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; //��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.  
    RCC_OscInitStructure.PLL.PLLP=pllp; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
	
    if(ret!=HAL_OK) while(1);
    
    ret=HAL_PWREx_EnableOverDrive(); //����Over-Driver����
    if(ret!=HAL_OK) while(1);
    
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2��Ƶϵ��Ϊ2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//ͬʱ����FLASH��ʱ����Ϊ5WS��Ҳ����6��CPU���ڡ�
		
    if(ret!=HAL_OK) while(1);
}

#ifdef  USE_FULL_ASSERT
//��������ʾ�����ʱ��˺����������������ļ���������
//file��ָ��Դ�ļ�
//line��ָ�����ļ��е�����
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI
__asm void DiableIt()
{
   CPSID I
   BX LR
}
__asm void EnableIt()
{
   CPSIE I
   BX LR
}


__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////

SPI_HandleTypeDef SPI1_Handler;
SPI_HandleTypeDef SPI2_Handler;
SPI_HandleTypeDef SPI3_Handler;
SPI_HandleTypeDef SPI4_Handler;
SPI_HandleTypeDef SPI5_Handler;
/////////////////////////////////////////////////////////////
//SPI5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_SPI_Init()����
//hspi:SPI���
/////////////////////////////////////////////////////////////
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();       //ʹ��GPIOBʱ��
    __HAL_RCC_SPI2_CLK_ENABLE();        //ʹ��SPI2ʱ��
    
    //PB13,14,15  
    GPIO_Initure.Pin=				GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;   ///GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=			GPIO_MODE_AF_PP;              				 //�����������
    GPIO_Initure.Pull=			GPIO_PULLUP;                            //����
    GPIO_Initure.Speed=			GPIO_SPEED_FAST;                        //����            
    GPIO_Initure.Alternate=	GPIO_AF5_SPI2;                         //����ΪSPI2
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	//////////////////////////////////////////////////////////////////////////////////
	  /////////////////////////////////////////////////////////////////////////////////
   __HAL_RCC_GPIOF_CLK_ENABLE();       //ʹ��GPIOFʱ��
    __HAL_RCC_SPI5_CLK_ENABLE();        //ʹ��SPI2ʱ��
    
    //PF7,8,9  
    GPIO_Initure.Pin=				GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=			GPIO_MODE_AF_PP;              				 //�����������
    GPIO_Initure.Pull=			GPIO_PULLUP;                            //����
    GPIO_Initure.Speed=			GPIO_SPEED_FAST;                        //����            
    GPIO_Initure.Alternate=	GPIO_AF5_SPI5;                         //����ΪSPI5
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);	
}



/////////////////////////////////////////////////////////////
// SPI�ٶ����ú���
// SPI�ٶ�=fAPB1/��Ƶϵ��
// @ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
// fAPB1ʱ��һ��Ϊ45Mhz��
/////////////////////////////////////////////////////////////

void SPI_SetSpeed(u8 SPI_BaudRatePrescaler,u8 uwSPI_No)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//�ж���Ч��
	
		if(uwSPI_No == 1)
		{
			__HAL_SPI_DISABLE(&SPI1_Handler);            										//�ر�SPI
			SPI1_Handler.Instance->CR1&=0XFFC7;          										//λ3-5���㣬�������ò�����
			SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//����SPI�ٶ�   0: f_pclk = clock_APB_2 = 168/32 mHz = 5.25Mhz
			__HAL_SPI_ENABLE(&SPI1_Handler);             										//ʹ��SPI		
		}
		else if(uwSPI_No == 2)
		{
				__HAL_SPI_DISABLE(&SPI2_Handler);            										//�ر�SPI
			SPI2_Handler.Instance->CR1&=0XFFC7;          										//λ3-5���㣬�������ò�����
			SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//����SPI�ٶ�
			__HAL_SPI_ENABLE(&SPI2_Handler);             										//ʹ��SPI	
		}
		else if(uwSPI_No == 3)
		{
				__HAL_SPI_DISABLE(&SPI3_Handler);            										//�ر�SPI
			SPI3_Handler.Instance->CR1&=0XFFC7;          										//λ3-5���㣬�������ò�����
			SPI3_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//����SPI�ٶ�
			__HAL_SPI_ENABLE(&SPI3_Handler);             										//ʹ��SPI	
		}
		else if(uwSPI_No == 4)
		{
				__HAL_SPI_DISABLE(&SPI4_Handler);            										//�ر�SPI
			SPI4_Handler.Instance->CR1&=0XFFC7;          										//λ3-5���㣬�������ò�����
			SPI4_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//����SPI�ٶ�
			__HAL_SPI_ENABLE(&SPI4_Handler);             										//ʹ��SPI	
		} 
		else if(uwSPI_No == 5)
		{
				__HAL_SPI_DISABLE(&SPI5_Handler);            										//�ر�SPI
			SPI5_Handler.Instance->CR1&=0XFFC7;          										//λ3-5���㣬�������ò�����
			SPI5_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//����SPI�ٶ�
			__HAL_SPI_ENABLE(&SPI5_Handler);             										//ʹ��SPI	
		}   		
}




/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////
void SPI_1_Init()
{
    SPI1_Handler.Instance							= SPI1;                         	//SPI  --- 1
    SPI1_Handler.Init.Mode						= SPI_MODE_MASTER;             		//����SPI����ģʽ������Ϊ��ģʽ
    SPI1_Handler.Init.Direction				= SPI_DIRECTION_2LINES;   				//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI1_Handler.Init.DataSize				= SPI_DATASIZE_8BIT;       				//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI1_Handler.Init.CLKPolarity			= SPI_POLARITY_HIGH;    					//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI1_Handler.Init.CLKPhase				= SPI_PHASE_2EDGE;         				//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI1_Handler.Init.NSS							= SPI_NSS_SOFT;                 	//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI1_Handler.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI1_Handler.Init.FirstBit					= SPI_FIRSTBIT_MSB;        			//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI1_Handler.Init.TIMode						= SPI_TIMODE_DISABLE;        		//�ر�TIģʽ
    SPI1_Handler.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLE;		//�ر�Ӳ��CRCУ��
    SPI1_Handler.Init.CRCPolynomial			= 7;                  					//CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI1_Handler);												//��ʼ��
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //ʹ��SPI2
	
    //SPI1_ReadWriteByte(0Xff);                         //��������
	
	  SPI_SetSpeed(SPI_BAUDRATEPRESCALER_32,1);
}	

/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////
void SPI_2_Init()
{
    SPI2_Handler.Instance							= SPI2;                         	//SP2
    SPI2_Handler.Init.Mode						= SPI_MODE_MASTER;             		//����SPI����ģʽ������Ϊ��ģʽ
    SPI2_Handler.Init.Direction				= SPI_DIRECTION_2LINES;   				//����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI2_Handler.Init.DataSize				= SPI_DATASIZE_8BIT;       				//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI2_Handler.Init.CLKPolarity			= SPI_POLARITY_HIGH;    					//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI2_Handler.Init.CLKPhase				= SPI_PHASE_2EDGE;         				//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI2_Handler.Init.NSS							= SPI_NSS_SOFT;                 	//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI2_Handler.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI2_Handler.Init.FirstBit					= SPI_FIRSTBIT_MSB;        			//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI2_Handler.Init.TIMode						= SPI_TIMODE_DISABLE;        		//�ر�TIģʽ
    SPI2_Handler.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLE;		//�ر�Ӳ��CRCУ��
    SPI2_Handler.Init.CRCPolynomial			= 7;                  					//CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI2_Handler);												//��ʼ��
    
    __HAL_SPI_ENABLE(&SPI2_Handler);                    //ʹ��SPI2
	
    //SPI5_ReadWriteByte(0Xff);                         //��������
	
	  SPI_SetSpeed(SPI_BAUDRATEPRESCALER_32,2);

}	

/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////
void SPI_3_Init()
{

}	
/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////
void SPI_4_Init()
{

}	

/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////
void SPI_5_Init()
{
    SPI5_Handler.Instance=SPI5;                         //SP5
    SPI5_Handler.Init.Mode=SPI_MODE_MASTER;             //����SPI����ģʽ������Ϊ��ģʽ
    SPI5_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
    SPI5_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI5_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI5_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI5_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI5_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI5_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI5_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //�ر�TIģʽ
    SPI5_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//�ر�Ӳ��CRCУ��
    SPI5_Handler.Init.CRCPolynomial=7;                  //CRCֵ����Ķ���ʽ
    HAL_SPI_Init(&SPI5_Handler);//��ʼ��
    
    __HAL_SPI_ENABLE(&SPI5_Handler);                    //ʹ��SPI5
	
    //SPI5_ReadWriteByte(0Xff);                           //��������
	  SPI_SetSpeed(SPI_BAUDRATEPRESCALER_32,5);
	
	   //SPI5_ReadWriteByte(0Xad);                           //��������
}	
/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////
void SPI_Init()
{
	//SPI_1_Init();
	SPI_2_Init();
	//SPI_3_Init();
	//SPI_4_Init();
	SPI_5_Init();
}	


/////////////////////////////////////////////////////////////
///
////
////
/////////////////////////////////////////////////////////////
void STM32_Initial(void)
{
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
	  //CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS1_8TQ,6,CAN_MODE_LOOPBACK); //�ػ����Է�����CAN��ʼ��,������500Kbps   
		CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS1_8TQ,6,CAN_MODE_NORMAL);      //����ģʽ CAN��ʼ��,������500Kbps 
	
	  SPI_Init();
}

