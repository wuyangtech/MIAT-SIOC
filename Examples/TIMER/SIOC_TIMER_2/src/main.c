/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MIAT
* Version            : V1.0
* Date               : 03/19/2011
* Description        : Virtual Com Port Demo main file	: printf hello, world!
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "hw_config.h"
#include "stm32f10x_tim.h"


/* Private macro -------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Delay(vu32 nCount);
void TIM_Configuration(void);
void NVIC_Configuration(void);
void Stopwatch_counter(void);

/* Private define ------------------------------------------------------------*/
#define TIM2_IRQChannel              ((u8)0x1C)  /* TIM2 global Interrupt */
#define TIM3_IRQChannel              ((u8)0x1D)  /* TIM3 global Interrupt */
#define TIM4_IRQChannel              ((u8)0x1E)  /* TIM4 global Interrupt */

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

/* Private varible -----------------------------------------------------------*/
vu32 Stopwatch = 0;
vu32 State_flag = 0;

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{

  Set_System();
  getchar(); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  
  TIM_Configuration(); 
  NVIC_Configuration();

  printf("**********Enter your instruction**********\r\n");
  printf("1.Start to count\r\n");
  printf("2.Pause to count\r\n");
  printf("3.Continue to count\r\n");
  printf("4.Stop to count\r\n");

  while(1)
    {	
			
	}
}  
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
 
	/* TIM2 configuration */ 
  TIM_TimeBaseStructure.TIM_Period = 0x4AF;           
  TIM_TimeBaseStructure.TIM_Prescaler = 0xEA5F;        
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;     
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000; 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 	 //***
   
  TIM_OCStructInit(&TIM_OCInitStructure); 
  /* Output Compare Timing Mode configuration: Channel1 */ 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;                    
  TIM_OCInitStructure.TIM_Pulse = 0x0;   
  TIM_OC1Init(TIM2, &TIM_OCInitStructure); 	  //***
   
  /* TIM3 configuration */ 
  TIM_TimeBaseStructure.TIM_Period = 0x95F; 
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
  /* Output Compare Timing Mode configuration: Channel1 */ 
  TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
   
  /* TIM4 configuration */ 

 
  /* Output Compare Timing Mode configuration: Channel1 */ 

 
  /* TIM2 enable counter */ 
  TIM_Cmd(TIM2, ENABLE); 	  
  /* TIM3 enable counter */ 
  TIM_Cmd(TIM3, ENABLE); 
  /* TIM4 enable counter */ 


 
  /* Immediate load of TIM2 Precaler value */ 
  TIM_PrescalerConfig(TIM2, 0xEA5F, TIM_PSCReloadMode_Immediate); //***
  /* Immediate load of TIM3 Precaler value */   
  TIM_PrescalerConfig(TIM3, 0xEA5F, TIM_PSCReloadMode_Immediate); 
  /* Immediate load of TIM4 Precaler value */ 

 
  /* Clear TIM2 update pending flag */ 
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);  
  /* Clear TIM3 update pending flag */ 
  TIM_ClearFlag(TIM3, TIM_FLAG_Update); 
  /* Clear TIM4 update pending flag */ 
 

 
  /* Enable TIM2 Update interrupt */ 
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //***
  /* Enable TIM3 Update interrupt */ 
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
  /* Enable TIM4 Update interrupt */ 
 
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
void NVIC_Configuration(void)
{

  #ifdef  VECT_TAB_RAM   
  /* Set the Vector Table base location at 0x20000000 */  
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  
  #else  /* VECT_TAB_FLASH  */ 
  /* Set the Vector Table base location at 0x08000000 */  
  NVIC_SetVectorTable(0x08003000, 0x0);    
  #endif 

  
  /* Configure one bit for preemption priority */ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
 
  /* Enable the TIM2 Interrupt */ 
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure); 
   
  /* Enable the TIM3 Interrupt */ 
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_Init(&NVIC_InitStructure); 
  
  /* Enable the TIM4 Interrupt */ 
  
  
}
void Stopwatch_counter(void)
{
	//static int flag_start = 0;
	if( State_flag == 1 )
	{
	
	}
	else if(State_flag == 2)
	{	
	}
	else if(State_flag == 3)
	{
	
	}
	else if(State_flag == 4)
	{
	
	}
	printf("Now is %d ...\r\n", Stopwatch);


}	
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
