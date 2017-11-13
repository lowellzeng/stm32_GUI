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

//时钟系统配置函数
//Fvco=Fs*(plln/pllm);
//SYSCLK=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCO频率
//SYSCLK:系统时钟频率
//Fusb:USB,SDIO,RNG等的时钟频率
//Fs:PLL输入时钟频率,可以是HSI,HSE等. 
//plln:主PLL倍频系数(PLL倍频),取值范围:64~432.
//pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.

//外部晶振为25M的时候,推荐值:plln=360,pllm=25,pllp=2,pllq=8.
//得到:Fvco=25*(360/25)=360Mhz
//     SYSCLK=360/2=180Mhz
//     Fusb=360/8=45Mhz
//返回值:0,成功;1,失败
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟
    
    //下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    //时使性能与功耗实现平衡，此功能只有STM32F42xx和STM32F43xx器件有，
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; //主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; //主PLL倍频系数(PLL倍频),取值范围:64~432.  
    RCC_OscInitStructure.PLL.PLLP=pllp; //系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
	
    if(ret!=HAL_OK) while(1);
    
    ret=HAL_PWREx_EnableOverDrive(); //开启Over-Driver功能
    if(ret!=HAL_OK) while(1);
    
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2分频系数为2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//同时设置FLASH延时周期为5WS，也就是6个CPU周期。
		
    if(ret!=HAL_OK) while(1);
}

#ifdef  USE_FULL_ASSERT
//当编译提示出错的时候此函数用来报告错误的文件和所在行
//file：指向源文件
//line：指向在文件中的行数
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
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
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
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
//SPI5底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
/////////////////////////////////////////////////////////////
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
    __HAL_RCC_SPI2_CLK_ENABLE();        //使能SPI2时钟
    
    //PB13,14,15  
    GPIO_Initure.Pin=				GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;   ///GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=			GPIO_MODE_AF_PP;              				 //复用推挽输出
    GPIO_Initure.Pull=			GPIO_PULLUP;                            //上拉
    GPIO_Initure.Speed=			GPIO_SPEED_FAST;                        //快速            
    GPIO_Initure.Alternate=	GPIO_AF5_SPI2;                         //复用为SPI2
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	//////////////////////////////////////////////////////////////////////////////////
	  /////////////////////////////////////////////////////////////////////////////////
   __HAL_RCC_GPIOF_CLK_ENABLE();       //使能GPIOF时钟
    __HAL_RCC_SPI5_CLK_ENABLE();        //使能SPI2时钟
    
    //PF7,8,9  
    GPIO_Initure.Pin=				GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=			GPIO_MODE_AF_PP;              				 //复用推挽输出
    GPIO_Initure.Pull=			GPIO_PULLUP;                            //上拉
    GPIO_Initure.Speed=			GPIO_SPEED_FAST;                        //快速            
    GPIO_Initure.Alternate=	GPIO_AF5_SPI5;                         //复用为SPI5
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);	
}



/////////////////////////////////////////////////////////////
// SPI速度设置函数
// SPI速度=fAPB1/分频系数
// @ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
// fAPB1时钟一般为45Mhz：
/////////////////////////////////////////////////////////////

void SPI_SetSpeed(u8 SPI_BaudRatePrescaler,u8 uwSPI_No)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//判断有效性
	
		if(uwSPI_No == 1)
		{
			__HAL_SPI_DISABLE(&SPI1_Handler);            										//关闭SPI
			SPI1_Handler.Instance->CR1&=0XFFC7;          										//位3-5清零，用来设置波特率
			SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//设置SPI速度   0: f_pclk = clock_APB_2 = 168/32 mHz = 5.25Mhz
			__HAL_SPI_ENABLE(&SPI1_Handler);             										//使能SPI		
		}
		else if(uwSPI_No == 2)
		{
				__HAL_SPI_DISABLE(&SPI2_Handler);            										//关闭SPI
			SPI2_Handler.Instance->CR1&=0XFFC7;          										//位3-5清零，用来设置波特率
			SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//设置SPI速度
			__HAL_SPI_ENABLE(&SPI2_Handler);             										//使能SPI	
		}
		else if(uwSPI_No == 3)
		{
				__HAL_SPI_DISABLE(&SPI3_Handler);            										//关闭SPI
			SPI3_Handler.Instance->CR1&=0XFFC7;          										//位3-5清零，用来设置波特率
			SPI3_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//设置SPI速度
			__HAL_SPI_ENABLE(&SPI3_Handler);             										//使能SPI	
		}
		else if(uwSPI_No == 4)
		{
				__HAL_SPI_DISABLE(&SPI4_Handler);            										//关闭SPI
			SPI4_Handler.Instance->CR1&=0XFFC7;          										//位3-5清零，用来设置波特率
			SPI4_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//设置SPI速度
			__HAL_SPI_ENABLE(&SPI4_Handler);             										//使能SPI	
		} 
		else if(uwSPI_No == 5)
		{
				__HAL_SPI_DISABLE(&SPI5_Handler);            										//关闭SPI
			SPI5_Handler.Instance->CR1&=0XFFC7;          										//位3-5清零，用来设置波特率
			SPI5_Handler.Instance->CR1|=SPI_BaudRatePrescaler;							//设置SPI速度
			__HAL_SPI_ENABLE(&SPI5_Handler);             										//使能SPI	
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
    SPI1_Handler.Init.Mode						= SPI_MODE_MASTER;             		//设置SPI工作模式，设置为主模式
    SPI1_Handler.Init.Direction				= SPI_DIRECTION_2LINES;   				//设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize				= SPI_DATASIZE_8BIT;       				//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI1_Handler.Init.CLKPolarity			= SPI_POLARITY_HIGH;    					//串行同步时钟的空闲状态为高电平
    SPI1_Handler.Init.CLKPhase				= SPI_PHASE_2EDGE;         				//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI1_Handler.Init.NSS							= SPI_NSS_SOFT;                 	//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_256;		//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit					= SPI_FIRSTBIT_MSB;        			//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode						= SPI_TIMODE_DISABLE;        		//关闭TI模式
    SPI1_Handler.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLE;		//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial			= 7;                  					//CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);												//初始化
    
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI2
	
    //SPI1_ReadWriteByte(0Xff);                         //启动传输
	
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
    SPI2_Handler.Init.Mode						= SPI_MODE_MASTER;             		//设置SPI工作模式，设置为主模式
    SPI2_Handler.Init.Direction				= SPI_DIRECTION_2LINES;   				//设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI2_Handler.Init.DataSize				= SPI_DATASIZE_8BIT;       				//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI2_Handler.Init.CLKPolarity			= SPI_POLARITY_HIGH;    					//串行同步时钟的空闲状态为高电平
    SPI2_Handler.Init.CLKPhase				= SPI_PHASE_2EDGE;         				//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI2_Handler.Init.NSS							= SPI_NSS_SOFT;                 	//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_Handler.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_256;		//定义波特率预分频的值:波特率预分频值为256
    SPI2_Handler.Init.FirstBit					= SPI_FIRSTBIT_MSB;        			//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_Handler.Init.TIMode						= SPI_TIMODE_DISABLE;        		//关闭TI模式
    SPI2_Handler.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLE;		//关闭硬件CRC校验
    SPI2_Handler.Init.CRCPolynomial			= 7;                  					//CRC值计算的多项式
    HAL_SPI_Init(&SPI2_Handler);												//初始化
    
    __HAL_SPI_ENABLE(&SPI2_Handler);                    //使能SPI2
	
    //SPI5_ReadWriteByte(0Xff);                         //启动传输
	
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
    SPI5_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI5_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI5_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI5_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI5_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI5_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI5_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI5_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI5_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI5_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI5_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI5_Handler);//初始化
    
    __HAL_SPI_ENABLE(&SPI5_Handler);                    //使能SPI5
	
    //SPI5_ReadWriteByte(0Xff);                           //启动传输
	  SPI_SetSpeed(SPI_BAUDRATEPRESCALER_32,5);
	
	   //SPI5_ReadWriteByte(0Xad);                           //启动传输
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
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
	  //CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS1_8TQ,6,CAN_MODE_LOOPBACK); //回环，自发自收CAN初始化,波特率500Kbps   
		CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS1_8TQ,6,CAN_MODE_NORMAL);      //正常模式 CAN初始化,波特率500Kbps 
	
	  SPI_Init();
}

